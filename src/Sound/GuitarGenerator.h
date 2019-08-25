#pragma once

#include "SoundGenerator.h"

#include <QObject>

class GuitarGenerator : public SoundGenerator
{
    Q_OBJECT
public:
    explicit GuitarGenerator(QObject* parent = nullptr);

    void playHarmony(const Music::Harmony& ) override;
};
