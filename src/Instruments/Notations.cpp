#include "Notations.h"

namespace Instruments {

bool operator == (const GuitarNotation& lhs, const GuitarNotation& rhs)
{
    return lhs.string == rhs.string && lhs.fret == rhs.fret;
}

bool operator > (const GuitarNotation& lhs, const GuitarNotation& rhs)
{
    if(lhs.string != rhs.string)
        return lhs.fret > rhs.fret;

    return lhs.fret > rhs.fret;
}

bool operator < (const GuitarNotation& lhs, const GuitarNotation& rhs)
{
    if(lhs.string != rhs.string)
        return lhs.fret < rhs.fret;

    return lhs.fret < rhs.fret;
}

}
