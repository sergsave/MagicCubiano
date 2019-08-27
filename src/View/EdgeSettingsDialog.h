#pragma once

#include <QDialog>
#include <QPixmap>
#include <src/Music.h>

class EdgeSettingsDialog: public QDialog
{
    Q_OBJECT
public:
    EdgeSettingsDialog(const QPixmap& icon, QWidget * parent = nullptr) :
        QDialog(parent), m_icon(icon)
    {}

    virtual Music::Harmony harmony() const = 0;

protected:
    QPixmap icon() const { return m_icon; }

private:
    QPixmap m_icon;
};
