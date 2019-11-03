#pragma once

#include <QObject>

class Model;

class SoundView : public QObject
{
public:
    SoundView(Model const * model, QObject * parent = nullptr);

private:
    Model const * m_model;
};

