#include "GuitarGenerators.h"

#include <QString>
#include "src/Instruments/Common.h"

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

GuitarGenerator::GuitarGenerator(QObject * parent):
    SampleGenerator(
        [] (auto tone){ return resourcePath(tone, "guitar"); },
        Instruments::defineInterval<Instruments::GuitarTag>(),
        parent)
{}

ElectricGuitarGenerator::ElectricGuitarGenerator(QObject * parent):
    SampleGenerator(
        [] (auto tone){ return resourcePath(tone, "distortion_guitar"); },
        Instruments::defineInterval<Instruments::ElectricGuitarTag>(),
        parent)
{}

