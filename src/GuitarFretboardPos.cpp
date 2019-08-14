#include "GuitarFretboardPos.h"

#include <QVector>

#include <cassert>

namespace {

// http://static1.squarespace.com/static/57a08f8a414fb546e8e35178/587dae1f9f7456b5f7fc5413/59e456f08dd041f0a6c9ed93/1508217399278/Guitar_fret_notes_and_frequency_hz.png?format=1500w
const QVector<QVector<double>> g_frequenciesTable
{
    // All frets for first string
    {
      82.41 , 87.3  , 92.49 , 97.99 ,
      103.83, 110.00, 116.54, 123.47,
      130.81, 138.59, 146.83, 155.56,
      164.81, 174.61
    },

    {
      110.0 , 116.54, 123.47, 130.81,
      138.59, 146.83, 155.56, 164.81,
      174.61, 185.00, 196.00, 207.65,
      220.00, 233.08
    },

    {
      146.83, 155.56, 164.81, 174.61,
      185.00, 196.00, 207.65, 220.00,
      233.08, 246.94, 261.63, 277.18,
      293.66, 311.13
    },

    {
      196.00, 207.65, 220.00, 233.08,
      246.94, 261.63, 277.18, 296.66,
      311.13, 329.63, 349.23, 369.99,
      392.00, 415.30
    },

    {
      246.94, 261.63, 277.18, 293.66,
      311.13, 329.63, 349.23, 369.99,
      392.00, 415.30, 440.00, 466.16,
      493.88, 523.25
    },

    {
      329.63, 349.23, 369.99, 392.00,
      415.30, 440.00, 466.16, 493.88,
      523.25, 554.37, 587.33, 622.25,
      659.2 , 698.46
    },
};

}

double GuitarFretboardPos::frequency() const
{
    // TODO: constexpr and static_assert
    assert(g_frequenciesTable.size() == GuitarFretboardPos::maxString && "Not full table!");

    for(auto & a: g_frequenciesTable)
        assert(a.size() == (GuitarFretboardPos::maxFret + 1) && "Not full table!");

    auto stringIdx = string - 1;
    auto fretIdx = fret;

    if(stringIdx < 0 || stringIdx >= g_frequenciesTable.size())
        return 0;

    auto stringFreqs = g_frequenciesTable.at(stringIdx);

    if(fretIdx < 0 || fretIdx >= stringFreqs.size())
        return 0;

    return stringFreqs.at(fretIdx);
}
