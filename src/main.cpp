#include <QApplication>
#include <QScopedPointer>

#include "Protocol/GiikerProtocol.h"
#include "Sound/SoundGeneratorFactory.h"
#include "View/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // order is important
    QScopedPointer<SoundGenerator> soundGenerator;
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

    auto updateGenerator = [&soundGenerator] (Music::Instrument inst) {
        soundGenerator.reset(createSoundGenerator(inst));
    };

    updateGenerator(window.instrumentType());
    QObject::connect(&window, &MainWindow::instrumentTypeChanged, updateGenerator);

    QObject::connect(&protocol, &GiikerProtocol::cubeEdgeTurned, &window,
        [&soundGenerator, &window] (const CubeEdge& info) {

        window.highlightEdge(info.color);

        if(soundGenerator)
            soundGenerator->play(window.harmonyFor(info), window.volume());
    });

    window.start();

    return a.exec();
}
