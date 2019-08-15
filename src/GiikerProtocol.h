#pragma once

#include <QObject>
#include <QLowEnergyService>
#include <QBluetoothDeviceInfo>

#include "MagicCubianoGlobal.h"

class QBluetoothDeviceDiscoveryAgent;
class QLowEnergyController;

class GiikerProtocol : public QObject
{
    Q_OBJECT

public:
    explicit GiikerProtocol(QObject *parent = 0);

    void connectToCube();
    // TODO: by name
    void connectToCube(const QString& macAddress);

signals:
    void cubeConnected();
    void cubeConnectionFailed();
    void cubeEdgeTurned(const CubeEdge& info);

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

