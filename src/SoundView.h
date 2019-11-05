#pragma once

#include <QObject>

class Model;
class SoundGenerator;

class SoundView : public QObject
{
public:
    SoundView(Model const * model, QObject * parent = nullptr);

private:
    Model const * m_model = nullptr;
    SoundGenerator * m_generator = nullptr;
};

