#include "ToneGenerator.h"

#include <QIODevice>
#include <QAudioOutput>
#include <QDebug>
#include <QVector>
#include <qmath.h>
#include <qendian.h>

namespace {

qreal frequencyFor(const Music::Tone& tone)
{
    // http://pages.mtu.edu/~suits/notefreqs.html
    static const QVector<qreal> freqs
    {
        16.35, 17.32, 18.35, 19.45, 20.60, 21.83,  23.12, 24.50, 25.96, 27.50, 29.14, 30.87
    };

    static const auto tones = Music::allTonesForOctave(0);

    const auto octave0Tone = Music::Tone(tone.note, 0);
    const int idx = tones.indexOf(octave0Tone);

    return freqs[idx] * qPow(2, tone.octave);
}

}

// From Qt example
class Generator : public QIODevice
{
public:
    Generator(const QAudioFormat &format,
                         qint64 durationUs,
                         int sampleRate,
                         QObject *parent)
        :   QIODevice(parent)
        ,   m_pos(0)
    {
        if (format.isValid())
            generateData(format, durationUs, sampleRate);
    }

    void start()
    {
        open(QIODevice::ReadOnly);
    }

    void stop()
    {
        m_pos = 0;
        close();
    }

    qint64 readData(char *data, qint64 len) override
    {
        qint64 total = 0;
        if (!m_buffer.isEmpty()) {
            while (len - total > 0) {
                const qint64 chunk = qMin((m_buffer.size() - m_pos), len - total);
                memcpy(data + total, m_buffer.constData() + m_pos, chunk);
                m_pos = (m_pos + chunk) % m_buffer.size();
                total += chunk;
            }
        }
        return total;
    }

    qint64 writeData(const char *data, qint64 len) override
    {
        Q_UNUSED(data);
        Q_UNUSED(len);

        return 0;
    }

    qint64 bytesAvailable() const override
    {
        return m_buffer.size() + QIODevice::bytesAvailable();
    }

private:
    void generateData(const QAudioFormat &format, qint64 durationUs, int sampleRate)
    {
        const int channelBytes = format.sampleSize() / 8;
        const int sampleBytes = format.channelCount() * channelBytes;

        qint64 length = (format.sampleRate() * format.channelCount() * (format.sampleSize() / 8))
                            * durationUs / 100000;

        Q_ASSERT(length % sampleBytes == 0);
        Q_UNUSED(sampleBytes) // suppress warning in release builds

        m_buffer.resize(length);
        unsigned char *ptr = reinterpret_cast<unsigned char *>(m_buffer.data());
        int sampleIndex = 0;

        while (length) {
            const qreal x = qSin(2 * M_PI * sampleRate * qreal(sampleIndex % format.sampleRate()) / format.sampleRate());
            for (int i=0; i<format.channelCount(); ++i) {
                if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::UnSignedInt) {
                    const quint8 value = static_cast<quint8>((1.0 + x) / 2 * 255);
                    *reinterpret_cast<quint8*>(ptr) = value;
                } else if (format.sampleSize() == 8 && format.sampleType() == QAudioFormat::SignedInt) {
                    const qint8 value = static_cast<qint8>(x * 127);
                    *reinterpret_cast<quint8*>(ptr) = value;
                } else if (format.sampleSize() == 16 && format.sampleType() == QAudioFormat::UnSignedInt) {
                    quint16 value = static_cast<quint16>((1.0 + x) / 2 * 65535);
                    if (format.byteOrder() == QAudioFormat::LittleEndian)
                        qToLittleEndian<quint16>(value, ptr);
                    else
                        qToBigEndian<quint16>(value, ptr);
                } else if (format.sampleSize() == 16 && format.sampleType() == QAudioFormat::SignedInt) {
                    qint16 value = static_cast<qint16>(x * 32767);
                    if (format.byteOrder() == QAudioFormat::LittleEndian)
                        qToLittleEndian<qint16>(value, ptr);
                    else
                        qToBigEndian<qint16>(value, ptr);
                }

                ptr += channelBytes;
                length -= channelBytes;
            }
            ++sampleIndex;
        }
    }

private:
    qint64 m_pos;
    QByteArray m_buffer;

};

ToneGenerator::ToneGenerator(QObject * parent)
    :   SoundGenerator(parent)
    ,   m_device(QAudioDeviceInfo::defaultOutputDevice())
    ,   m_audioOutput(nullptr)
    ,   m_generator(nullptr)
{
}

void ToneGenerator::initializeAudio(int hz, int msec)
{
    const int dataSampleRateHz = 44100;

    m_format.setSampleRate(dataSampleRateHz);
    m_format.setChannelCount(1);
    m_format.setSampleSize(16);
    m_format.setCodec("audio/pcm");
    m_format.setByteOrder(QAudioFormat::LittleEndian);
    m_format.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info(m_device);
    if (!info.isFormatSupported(m_format)) {
        qWarning() << "Default format not supported - trying to use nearest";
        m_format = info.nearestFormat(m_format);
    }

    if (m_generator)
    {
        delete m_generator;
        m_generator = nullptr;
    }
    m_generator = new Generator(m_format, msec*1000, hz, this);

    createAudioOutput();
}

void ToneGenerator::createAudioOutput()
{
    if(m_audioOutput)
    {
        delete m_audioOutput;
        m_audioOutput = nullptr;
    }

    m_audioOutput = new QAudioOutput(m_device, m_format, this);
    m_generator->start();
    m_audioOutput->start(m_generator);
}

void ToneGenerator::stop()
{
    if(m_generator)
        m_generator->stop();

    if(m_audioOutput)
    {
        m_audioOutput->stop();
        m_audioOutput->disconnect(this);
    }
}

void ToneGenerator::playTones()
{
    stop();

    if(m_harmonyCounter >= m_harmony.tones.size())
        return;

    auto freq = frequencyFor(m_harmony.tones[m_harmonyCounter++]);
    initializeAudio(freq, m_harmony.delayMSec);
    m_audioOutput->setNotifyInterval(m_harmony.delayMSec);
    connect(m_audioOutput, &QAudioOutput::notify, this, &ToneGenerator::playTones);
}

void ToneGenerator::playHarmony(const Music::Harmony & harmony)
{
    if (harmony.tones.empty())
        return;

    m_harmonyCounter = 0;
    m_harmony = harmony;

    // TODO: play without stop
    playTones();
}
