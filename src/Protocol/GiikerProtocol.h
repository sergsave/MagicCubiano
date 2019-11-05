#pragma once

#include <QObject>
#include <QLowEnergyService>
#include <QBluetoothDeviceInfo>

#include "src/CubeEdge.h"

class QBluetoothDeviceDiscoveryAgent;
class QLowEnergyController;

class GiikerProtocol : public QObject
{
    Q_OBJECT

public:
    explicit GiikerProtocol(QObject *parent = 0);

    enum State
    {
        CONNECTED,
        DISCONNECTED
    };

    State state() const;

public slots:
    // Can connect to one cube in time only
    void connectToCube();
    void connectToCubeByAddress(const QString& macAddress); // TODO: by name
    void cancelConnection();
    void disconnectFromCube();

    void requestBatteryLevel();

signals:
    void connected();
    void connectionFailed();
    void connectionCanceled();
    void disconnected();

    void cubeEdgeTurned(const CubeEdge& info);
    void batteryLevelResponsed(int batteryInPerc);

private slots:
    void connectToDevice(const QBluetoothDeviceInfo& device);
    void serviceScanDone();
    void serviceStateChanged(QLowEnergyService::ServiceState st);
    void handleCharacteristicData(const QLowEnergyCharacteristic &c, const QByteArray &value);
    void stopDiscovery();
    void resetBleControl();

    QLowEnergyService * createService(const QBluetoothUuid& uuid);
    void deleteAllServices();
    void handleCubeCharData(const QByteArray &value);
    void handleBatteryCharData(const QByteArray &value);

private:
    QBluetoothDeviceDiscoveryAgent * m_discoveryDevAgent = nullptr;
    QLowEnergyController * m_bleController = nullptr;

    QLowEnergyService * m_cubeService = nullptr;
    QLowEnergyService * m_batteryService = nullptr;
};

