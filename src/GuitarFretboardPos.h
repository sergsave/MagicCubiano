#ifndef GUITARFRETBOARDPOS_H
#define GUITARFRETBOARDPOS_H

struct GuitarFretboardPos
{
    // Min string is 1, min fret is 0 (open string)
    GuitarFretboardPos(int string = 1, int fret = 0) :
        string(string), fret(fret) {}

    double frequency() const;

    int string;
    int fret;

    static const int maxString = 6;
    static const int maxFret = 13;
};

#endif // GUITARFRETBOARDPOS_H
