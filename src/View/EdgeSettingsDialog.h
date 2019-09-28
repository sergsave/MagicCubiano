#pragma once

#include <QDialog>
#include <QPixmap>
#include <src/Music.h>

class EdgeSettingsDialog: public QDialog
{
    Q_OBJECT
public:
    EdgeSettingsDialog(QWidget * parent = nullptr) :
        QDialog(parent)
    {}

    virtual void setIcon(const QPixmap& pm) = 0;
    virtual Music::Harmony harmony() const = 0;
    virtual void setHarmony(const Music::Harmony&) = 0;
};
