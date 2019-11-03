#include "GiikerProtocol.h"

#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>
#include <QTimer>

namespace {

const QString g_cubeServiceUuid = "{0000aadb-0000-1000-8000-00805f9b34fb}";
const QString g_cubeServiceCharUuid = "{0000aadc-0000-1000-8000-00805f9b34fb}";
const QString g_cubeServiceNotifyDescUuid = "{00002902-0000-1000-8000-00805f9b34fb}";

void enableCharNotifications(QLowEnergyService * serv, const QBluetoothUuid &charUuid, const QBluetoothUuid& descUuid)
{
    if(!serv)
        return;

    auto characteristic = serv->characteristic(charUuid);
    auto notificationDesc = characteristic.descriptor(descUuid);

    if (notificationDesc.isValid())
        serv->writeDescriptor(notificationDesc, QByteArray::fromHex("0100"));
}

}

GiikerProtocol::GiikerProtocol(QObject * parent):
    QObject(parent)
{
    m_discoveryDevAgent = new QBluetoothDeviceDiscoveryAgent(this);
    m_discoveryDevAgent->setLowEnergyDiscoveryTimeout(0);

    auto deviceDiscovered = [this] (const QBluetoothDeviceInfo & device){
        if(device.name().startsWith("Gi"))
            connectToDevice(device);
    };

    using Error = QBluetoothDeviceDiscoveryAgent::Error;

    auto errorOccured = [this](Error err) {
            Q_UNUSED(err);
            emit connectionFailed();
    };

    connect(m_discoveryDevAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, deviceDiscovered);

    connect(m_discoveryDevAgent, QOverload<Error>::of(&QBluetoothDeviceDiscoveryAgent::error),
            this, errorOccured);
}

void GiikerProtocol::connectToCube()
{
    m_discoveryDevAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void GiikerProtocol::connectToCubeByAddress(const QString &macAddres)
{
    if(QBluetoothLocalDevice().hostMode() == QBluetoothLocalDevice::HostPoweredOff)
    {
        QTimer::singleShot(0, this, &GiikerProtocol::connectionFailed);
        return;
    }

    QBluetoothDeviceInfo info(QBluetoothAddress(macAddres), "", 0);
    connectToDevice(info);
}

void GiikerProtocol::stopDiscovery()
{
    if(m_discoveryDevAgent && m_discoveryDevAgent->isActive())
        m_discoveryDevAgent->stop();
}

GiikerProtocol::State GiikerProtocol::state() const
{
    if(m_bleController && m_bleController->state() == QLowEnergyController::ConnectedState)
        return CONNECTED;

    return DISCONNECTED;
}

void GiikerProtocol::cancelConnection()
{
    stopDiscovery();

    if(m_bleController && m_bleController->state() != QLowEnergyController::ConnectedState)
        m_bleController->deleteLater();
}

void GiikerProtocol::disconnectFromCube()
{
    // TODO: reset service descriptors

    if(!m_bleController)
        return;

    m_bleController->disconnectFromDevice();
    delete m_bleController;
    m_bleController = nullptr;
}

void GiikerProtocol::connectToDevice(const QBluetoothDeviceInfo &device)
{
    stopDiscovery();
    disconnectFromCube();

    m_bleController = new QLowEnergyController(device.address(), this);
    m_bleController->setRemoteAddressType(QLowEnergyController::PublicAddress);

    connect(m_bleController, &QLowEnergyController::connected, this, [this] {
        m_bleController->discoverServices();
    });

    connect(m_bleController, &QLowEnergyController::disconnected, this, [this] {
        emit disconnected();
    });

    connect(m_bleController, &QLowEnergyController::discoveryFinished, this,
        &GiikerProtocol::serviceScanDone);

    m_bleController->connectToDevice();
}

QLowEnergyService * GiikerProtocol::createService(const QBluetoothUuid &uuid)
{
    auto service = m_bleController->createServiceObject(uuid, this);

    if(!service)
        return nullptr;

    connect(service, &QLowEnergyService::stateChanged,
        this, &GiikerProtocol::serviceStateChanged);

    connect(service, &QLowEnergyService::characteristicChanged,
        this, &GiikerProtocol::onCharacteristicChanged);

    service->discoverDetails();
    return service;
}

void GiikerProtocol::deleteAllServices()
{
    auto reset = [] (QLowEnergyService*& service) {
        if(service)
        {
            delete service;
            service = nullptr;
        }
    };
    reset(m_cubeService);
    reset(m_batteryService);
}

void GiikerProtocol::serviceScanDone()
{
    deleteAllServices();

    m_cubeService = createService(QBluetoothUuid(g_cubeServiceUuid));
    m_batteryService = createService(QBluetoothUuid(QBluetoothUuid::BatteryService));

    if(m_cubeService && m_batteryService)
        emit connected();
    else
        emit connectionFailed();

}

void GiikerProtocol::serviceStateChanged(QLowEnergyService::ServiceState st)
{
    switch (st)
    {
    case QLowEnergyService::ServiceDiscovered:
    {
        enableCharNotifications(m_cubeService,
            QBluetoothUuid(g_cubeServiceCharUuid),
            QBluetoothUuid(g_cubeServiceNotifyDescUuid));
        enableCharNotifications(m_batteryService,
            { QBluetoothUuid::CharacteristicType::BatteryLevel },
            { QBluetoothUuid::ClientCharacteristicConfiguration });
    }
        break;
    default:
        break;
    }
}

void GiikerProtocol::onCubeCharChanged(const QByteArray &value)
{
    const QMap<char, CubeEdge::Color> code2edges
    {
        {1, CubeEdge::BLUE},
        {2, CubeEdge::YELLOW},
        {3, CubeEdge::ORANGE},
        {4, CubeEdge::WHITE},
        {5, CubeEdge::RED},
        {6, CubeEdge::GREEN}
    };

    const QMap<char, CubeEdge::Rotation> code2direction
    {
        {1, CubeEdge::CLOCKWIZE},
        {3, CubeEdge::ANTICLOCKWIZE}
    };

    QByteArray lastMovesCode = value.right(4);
    char lastestMoveCode = lastMovesCode[0];

    char highHalfByte = lastestMoveCode / 16;
    char lowHalfByte = lastestMoveCode % 16;

    if(!code2edges.contains(highHalfByte) || !code2direction.contains(lowHalfByte))
        return;

    qDebug() << "cubeEdgeTurned";

    CubeEdge info;
    info.color = code2edges.value(highHalfByte);
    info.rotation = code2direction.value(lowHalfByte);

    emit cubeEdgeTurned(info);
}

void GiikerProtocol::onBatteryCharChanged(const QByteArray &value)
{
    // TODO:
}

void GiikerProtocol::onCharacteristicChanged(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    if (c.uuid().toString() == g_cubeServiceCharUuid)
        onCubeCharChanged(value);

    if(c.uuid() == QBluetoothUuid(QBluetoothUuid::BatteryLevel))
        onBatteryCharChanged(value);
}

void GiikerProtocol::requestBatteryLevel()
{
    // TODO:
}

