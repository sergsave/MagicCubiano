#pragma once

#include <QDialog>
#include <src/Music.h>

class EdgeSettingsDialog: public QDialog
{
    Q_OBJECT
public:
    using QDialog::QDialog;

    virtual Music::Harmony harmony() const = 0;
};
