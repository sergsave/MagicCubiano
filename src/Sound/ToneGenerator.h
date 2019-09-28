#pragma once

#include <QAudioFormat>
#include <QAudioDeviceInfo>

#include "SoundGenerator.h"

class QIODevice;
class QAudioOutput;

class Generator;

// TODO: Play harmony without stop
// TODO: Volume support
class ToneGenerator : public SoundGenerator
{
    Q_OBJECT

public:
    explicit ToneGenerator(const Music::Interval& , QObject* parent = nullptr);

private slots:
    void stop();
    void playTones();

private:
    void doPlay(const Music::Harmony&, int vol) override;
    void initializeAudio(int, int);
    void createAudioOutput();

private:
    QAudioDeviceInfo m_device;
    QAudioOutput *m_audioOutput;
    QAudioFormat m_format;
    Generator *m_generator;

    int m_harmonyCounter = 0;
    Music::Harmony m_harmony;
};

