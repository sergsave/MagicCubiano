#ifndef EDGESETTINGS_H
#define EDGESETTINGS_H

#include <QWidget>

namespace Ui {
class EdgeSettingsWidget;
}

class EdgeSettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EdgeSettingsWidget(QWidget *parent = 0);
    ~EdgeSettingsWidget();

    void setName(const QString& text);
    void setName(const QPixmap& pixmap);

    void setMaxStringNumber(int );
    void setMaxFretNumber(int );

    void setMinStringNumber(int );
    void setMinFretNumber(int );

    int currentString() const;
    int currentFret() const;

    void setCurrentString(int);
    void setCurrentFret(int);

private:
    Ui::EdgeSettingsWidget *ui;
};

#endif // EDGESETTINGS_H
