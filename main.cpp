#include <QApplication>

#include "GiikerProtocol.h"
#include "SoundGenerator.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow window;
    GiikerProtocol protocol;
    protocol.connectToCube();
    window.setStatus("Connecting...");

    QObject::connect(&protocol, &GiikerProtocol::cubeConnected, &window, [&window] {
        window.setStatus("Connected");
    });

    SoundGenerator soundGenerator;

    QObject::connect(&protocol, &GiikerProtocol::cubeEdgeTurned, &window,
        [&soundGenerator, &window] (auto info) {

        auto status = QString("Turned %1")
            .arg(info.rotation == CubeEdge::ANTICLOCKWIZE ? "'" : "");
        window.setStatus(status);

        auto duration = window.soundDuration();
        auto fretboardPos = window.guitarFretboardPosFor(info);
        auto freq = fretboardPos.frequency();

        soundGenerator.playSound(freq, duration);
    });

    window.show();

    return a.exec();
}
