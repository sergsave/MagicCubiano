#include <QApplication>

#include "GiikerProtocol.h"
#include "SoundGenerator.h"
#include "View/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window;
    GiikerProtocol protocol;

    QObject::connect(&window, &MainWindow::connectAnyRequested, &protocol,
        [&protocol]() { protocol.connectToCube();});
    QObject::connect(&window, &MainWindow::connectByAddressRequested, &protocol,
        [&protocol](auto addr) { protocol.connectToCube(addr);});

    QObject::connect(&protocol, &GiikerProtocol::cubeConnected,
        &window, &MainWindow::connected);
    QObject::connect(&protocol, &GiikerProtocol::cubeConnectionFailed,
        &window, &MainWindow::connectionFailed);

    SoundGenerator soundGenerator;

    QObject::connect(&protocol, &GiikerProtocol::cubeEdgeTurned, &soundGenerator,
        [&soundGenerator, &window] (const CubeEdge& info) {

        window.highlightEdge(info.color);
        // soundGenerator.playSound(window.harmonyFor(info.color));
    });

    window.start();

    return a.exec();
}
