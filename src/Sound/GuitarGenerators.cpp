#include "GuitarGenerators.h"

#include <QString>

namespace {

QString resourcePath(const Music::Tone& tone, const QString& folderName)
{
    auto copy = tone;
    copy.octave -= 2;

    QString prefix = ":/sounds/" + folderName + "/";
    QString postfix = ".wav";

    return prefix + copy.toString(Music::Tone::Format::IS) + postfix;
}

}

GuitarGenerator::GuitarGenerator(const Music::Interval &interval, QObject *parent):
    SampleGenerator(interval, parent)
{
    setResourcePathFunc([] (const Music::Tone& tone){ return resourcePath(tone, "guitar"); });
}

DistortionGuitarGenerator::DistortionGuitarGenerator(const Music::Interval &interval, QObject *parent):
    SampleGenerator(interval, parent)
{
    setResourcePathFunc([] (const Music::Tone& tone){ return resourcePath(tone, "distortion_guitar"); });
}

