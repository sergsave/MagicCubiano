#include "GiikerProtocol.h"

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
    m_discoveryDevAgent->setLowEnergyDiscoveryTimeout(40000);

    connect(m_discoveryDevAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &GiikerProtocol::connectToDevice);
}

void GiikerProtocol::connectToCube()
{
    m_discoveryDevAgent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}

void GiikerProtocol::connectToDevice(const QBluetoothDeviceInfo &device)
{
    qDebug() << device.name();
    if(!device.name().startsWith("GiC"))
        return;

    m_discoveryDevAgent->stop();

    // TODO: delete m_bleController

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

    const QMap<char, EdgeType> code2edges
    {
        {1, EdgeType::BACK},
        {2, EdgeType::UPPER},
        {3, EdgeType::RIGHT},
        {4, EdgeType::DOWN},
        {5, EdgeType::LEFT},
        {6, EdgeType::FRONT}
    };

    const QMap<char, TurnDirection> code2direction
    {
        {1, TurnDirection::CLOCKWIZE},
        {3, TurnDirection::ANTICLOKWISE}
    };

    QByteArray lastMovesCode = value.right(4);
    char lastestMoveCode = lastMovesCode[0];

    char highHalfByte = lastestMoveCode / 16;
    char lowHalfByte = lastestMoveCode % 16;

    if(!code2edges.contains(highHalfByte) || !code2direction.contains(lowHalfByte))
        return;

    qDebug() << "cubeEdgeTurned";

    TurnInfo turnInfo;
    turnInfo.edge = code2edges.value(highHalfByte);
    turnInfo.direction = code2direction.value(lowHalfByte);

    emit cubeEdgeTurned(turnInfo);
}

QString GiikerProtocol::TurnInfo::toString()
{
    const QMap<EdgeType, QString> edges2string
    {
        {EdgeType::BACK, "B"},
        {EdgeType::UPPER, "U"},
        {EdgeType::RIGHT, "R"},
        {EdgeType::DOWN, "D"},
        {EdgeType::LEFT, "L"},
        {EdgeType::FRONT, "F"}
    };

    const QMap<TurnDirection, QString> dir2string
    {
        {TurnDirection::CLOCKWIZE, ""},
        {TurnDirection::ANTICLOKWISE, "'"}
    };

    assert(edges2string.contains(edge));
    assert(dir2string.contains(direction));

    return QString("%1%2").arg(edges2string.value(edge), dir2string.value(direction));
}
