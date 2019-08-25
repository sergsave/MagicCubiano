#include "GiikerProtocol.h"

#include <QBluetoothLocalDevice>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>

#include <cassert>

const QString GiikerProtocol::serviceUuid =
        QString("{0000aadb-0000-1000-8000-00805f9b34fb}");
const QString GiikerProtocol::serviceCharUuid =
        QString("{0000aadc-0000-1000-8000-00805f9b34fb}");
const QString GiikerProtocol::serviceNotifyDescUuid =
        QString("{00002902-0000-1000-8000-00805f9b34fb}");

GiikerProtocol::GiikerProtocol(QObject * parent):
    QObject(parent)
{
    m_discoveryDevAgent = new QBluetoothDeviceDiscoveryAgent(this);

    // TODO: device selection dialog
    m_discoveryDevAgent->setLowEnergyDiscoveryTimeout(0);

    auto deviceDiscovered = [this] (const QBluetoothDeviceInfo & device){
        if(device.name().startsWith("Gi"))
            connectToDevice(device);
    };

    using Error = QBluetoothDeviceDiscoveryAgent::Error;

    auto errorOccured = [this](Error err) {
        if(err == Error::PoweredOffError)
            emit cubeConnectionFailed();
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

void GiikerProtocol::connectToCube(const QString &macAddres)
{
    if(QBluetoothLocalDevice().hostMode() == QBluetoothLocalDevice::HostPoweredOff)
    {
        emit cubeConnectionFailed();
        return;
    }

    QBluetoothDeviceInfo info(QBluetoothAddress(macAddres), "", 0);
    connectToDevice(info);
}

void GiikerProtocol::connectToDevice(const QBluetoothDeviceInfo &device)
{
    if(m_discoveryDevAgent && m_discoveryDevAgent->isActive())
        m_discoveryDevAgent->stop();

    m_bleController = new QLowEnergyController(device.address(), this);
    m_bleController->setRemoteAddressType(QLowEnergyController::PublicAddress);

    connect(m_bleController, &QLowEnergyController::connected, this, [this]() {
        m_bleController->discoverServices();
    });

    connect(m_bleController, &QLowEnergyController::discoveryFinished, this,
        &GiikerProtocol::serviceScanDone);

    // TODO: Quit the app after disconnect.
    // TODO: Auto connect.

    m_bleController->connectToDevice();
}

void GiikerProtocol::serviceScanDone()
{
    m_bleService = m_bleController->createServiceObject(QBluetoothUuid(serviceUuid), this);

    if(!m_bleService)
        return;

    emit cubeConnected();

    connect(m_bleService, &QLowEnergyService::stateChanged,
        this, &GiikerProtocol::serviceStateChanged);

    connect(m_bleService, &QLowEnergyService::characteristicChanged,
        this, &GiikerProtocol::onCharacteristicChanged);

    m_bleService->discoverDetails();
}

void GiikerProtocol::serviceStateChanged(QLowEnergyService::ServiceState st)
{
    switch (st)
    {
    case QLowEnergyService::ServiceDiscovered:
    {
        auto characteristic = m_bleService->characteristic(QBluetoothUuid(serviceCharUuid));
        auto notificationDesc = characteristic.descriptor(QBluetoothUuid(serviceNotifyDescUuid));

        if (notificationDesc.isValid())
            m_bleService->writeDescriptor(notificationDesc, QByteArray::fromHex("0100"));
    }
        break;
    default:
        break;
    }
}

void GiikerProtocol::onCharacteristicChanged(const QLowEnergyCharacteristic &c, const QByteArray &value)
{
    qDebug() << "BLE characteristic changed ";
    if (c.uuid().toString() != serviceCharUuid)
        return;

    qDebug() << "Wanted characteristic ";

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
