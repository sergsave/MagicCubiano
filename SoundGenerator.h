#ifndef SOUNDGENERATOR_H
#define SOUNDGENERATOR_H

#include <QObject>

#include <QAudioFormat>
#include <QAudioDeviceInfo>

class QIODevice;
class QAudioOutput;

class Generator;

class SoundGenerator : public QObject
{
    Q_OBJECT

public:
    explicit SoundGenerator(QObject* parent = 0);
    virtual ~SoundGenerator() = default;

    void playSound(int freqHz, int durMsec);

private slots:
    void stop();

private:
    void initializeAudio(int, int);
    void createAudioOutput();

private:
    QAudioDeviceInfo m_device;
    QAudioOutput *m_audioOutput;
    QAudioFormat m_format;
    Generator *m_generator;
};

#endif // SOUNDGENERATOR_H
