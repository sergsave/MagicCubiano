#pragma once


#include <QAudioFormat>
#include <QAudioDeviceInfo>

#include "SoundGenerator.h"

class QIODevice;
class QAudioOutput;

class Generator;

class ToneGenerator : public SoundGenerator
{
    Q_OBJECT

public:
    explicit ToneGenerator(QObject* parent = 0);
    virtual ~ToneGenerator() = default;

    void playHarmony(const Music::Harmony&) override;

private slots:
    void stop();
    void playTones();

private:
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

