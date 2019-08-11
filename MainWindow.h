#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

#include "CubeEdgeType.h"

namespace Ui {
class MainWindow;
}

// Now support only guitar mode
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    // TODO: Add abstract music info, and specific types for piano, notes etc.
    struct MusicInfo
    {
        MusicInfo(int str = 1, int fret = 0) : stringNumber(str), fretNumber(fret) {}

        int stringNumber;
        int fretNumber;
    };

    void setMaximumMusicInfo(const MusicInfo&);
    MusicInfo musicInfoFor(EdgeType type) const;

    int soundDuration() const;

public slots:
    void setStatus(const QString &text);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
