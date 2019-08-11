#ifndef GIIKERPROTOCOL_H
#define GIIKERPROTOCOL_H

#include <QBluetoothDeviceDiscoveryAgent>
#include <QLowEnergyController>

#include "CubeEdgeType.h"

class GiikerProtocol : public QObject
{
    Q_OBJECT

public:
    explicit GiikerProtocol(QObject *parent = 0);

    enum class TurnDirection
    {
        CLOCKWIZE,
        ANTICLOKWISE
    };

    struct TurnInfo
    {
        EdgeType edge = EdgeType::GREEN;
        TurnDirection direction = TurnDirection::CLOCKWIZE;
    };

    void connectToCube();

signals:
    bool cubeConnected();
    void cubeEdgeTurned(const TurnInfo& info);

private slots:
    void connectToDevice(const QBluetoothDeviceInfo& device);
    void serviceScanDone();
    void serviceStateChanged(QLowEnergyService::ServiceState st);
    void onCharacteristicChanged(const QLowEnergyCharacteristic &c, const QByteArray &value);

private:
    static const QString serviceUuid;
    static const QString serviceCharUuid;
    static const QString serviceNotifyDescUuid;

    QBluetoothDeviceDiscoveryAgent * m_discoveryDevAgent;
    QLowEnergyController * m_bleController;
    QLowEnergyService * m_bleService;
};

#endif // GIIKERPROTOCOL_H
