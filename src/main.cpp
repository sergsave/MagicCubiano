#include <QApplication>
#include <QScopedPointer>

#include "Protocol/GiikerProtocol.h"
#include "Sound/SoundGeneratorFactory.h"
#include "View/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // order is important
    MainWindow window;
    QScopedPointer<SoundGenerator> soundGenerator(createSoundGenerator(window.instrumentType()));
    GiikerProtocol protocol;

    QObject::connect(&window, &MainWindow::connectAnyRequested, &protocol,
        [&protocol]() { protocol.connectToCube();});
    QObject::connect(&window, &MainWindow::connectByAddressRequested, &protocol,
        [&protocol](auto addr) { protocol.connectToCube(addr);});

    QObject::connect(&protocol, &GiikerProtocol::cubeConnected,
        &window, &MainWindow::connected);
    QObject::connect(&protocol, &GiikerProtocol::cubeConnectionFailed,
        &window, &MainWindow::connectionFailed);

    QObject::connect(&window, &MainWindow::instrumentTypeChanged, [&soundGenerator] (Music::Instrument inst) {
        soundGenerator.reset(createSoundGenerator(inst));
    });

    QObject::connect(&protocol, &GiikerProtocol::cubeEdgeTurned, &window,
        [&soundGenerator, &window] (const CubeEdge& info) {

        window.highlightEdge(info.color);
        if(soundGenerator)
            soundGenerator->playHarmony(window.harmonyFor(info));
    });

    window.start();

    return a.exec();
}
