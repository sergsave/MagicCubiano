#include <QApplication>
#include <QScopedPointer>

#include "Protocol/GiikerProtocol.h"
#include "Sound/SoundGeneratorFactory.h"
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

    QScopedPointer<SoundGenerator> soundGenerator(createSoundGenerator(GenType::TONE));

    QObject::connect(&protocol, &GiikerProtocol::cubeEdgeTurned, soundGenerator.data(),
        [&soundGenerator, &window] (const CubeEdge& info) {

        window.highlightEdge(info.color);
        soundGenerator->playHarmony(window.harmonyFor(info));
    });

    window.start();

    return a.exec();
}
