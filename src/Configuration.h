#pragma once

#include <cassert>
#include <QList>

#include "Music.h"
#include "src/Instruments/Common.h"

// TODO: set in .ini or .txt
namespace Configuration
{

static QList<Instruments::Type> supportedInstruments()
{
    using Ins = Instruments::Type;
    return { Ins::GUITAR, Ins::ELECTRIC_GUITAR };
}

static QList<Instruments::Type> soonInstruments()
{
    using Ins = Instruments::Type;
    return { Ins::PIANO };
}

static int defaultMinNoteDurationMSec()
{
    return 0;
}

static int defaultVolume()
{
    return 100;
}

}
