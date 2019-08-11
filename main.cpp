#include <QApplication>

#include "GiikerProtocol.h"
#include "SoundGenerator.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    qint16 frequencyFrom(const MainWindow::MusicInfo&);

    QApplication a(argc, argv);

    MainWindow window;
    // TODO: All guitar strings and frets
    window.setMaximumMusicInfo({2, 12});

    GiikerProtocol protocol;
    protocol.connectToCube();
    window.setStatus("Connecting...");

    QObject::connect(&protocol, &GiikerProtocol::cubeConnected, &window, [&window] {
        window.setStatus("Connected");
    });

    SoundGenerator soundGenerator;

    QObject::connect(&protocol, &GiikerProtocol::cubeEdgeTurned, &window,
        [&soundGenerator, &window] (auto turnInfo) {

        auto status = QString("Turned %1")
            .arg(turnInfo.direction == GiikerProtocol::TurnDirection::ANTICLOKWISE ? "'" : "");
        window.setStatus(status);

        auto duration = window.soundDuration();
        auto musicInfo = window.musicInfoFor(turnInfo.edge);
        auto freq = frequencyFrom(musicInfo);

        soundGenerator.playSound(freq, duration);
    });

    window.show();

    return a.exec();
}

// Only 2 strings are supported now
qint16 frequencyFrom(const MainWindow::MusicInfo& musicInfo)
{
  // http://static1.squarespace.com/static/57a08f8a414fb546e8e35178/587dae1f9f7456b5f7fc5413/59e456f08dd041f0a6c9ed93/1508217399278/Guitar_fret_notes_and_frequency_hz.png?format=1500w
  QVector<QVector<qreal>> frequencies;

  // all frets for the first string
  frequencies.push_back(
  {
    82.41 , 87.3  , 92.49 , 97.99,
    103.83, 110.00, 116.54, 123.47,
    130.81, 138.59, 146.83, 155.56,
    293.66
  });

  // all frets for the second string
  frequencies.push_back(
  {
    110.0 , 116.54, 123.47, 130.81,
    138.59, 146.83, 155.56, 164.81,
    174.61, 185.00, 196.00, 207.65,
    220.00
  });

  auto stringIdx = musicInfo.stringNumber - 1;
  auto fretIdx = musicInfo.fretNumber;

  if(stringIdx < 0 || stringIdx >= frequencies.size())
      return 0;

  auto stringFreqs = frequencies.at(stringIdx);

  if(fretIdx < 0 || fretIdx >= stringFreqs.size())
      return 0;

  // TODO: Floating value freq
  return qRound(stringFreqs.at(fretIdx));
}
