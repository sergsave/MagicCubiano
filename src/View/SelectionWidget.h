#pragma once

#include <QWidget>
#include <QScopedPointer>

namespace Ui {
class SelectionWidget;
}

class SelectionWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SelectionWidget(QWidget *parent = nullptr);
    ~SelectionWidget();

    int index() const;

signals:
    void indexChanged(int);

protected:
    void setTitle(const QString&);
    QString title() const;

    // Reset index
    void setValues(const QStringList&);
    QStringList values() const;

private slots:
    void decrease();
    void increase();
    void onIndexUpdated(int);

private:
    int maxIndex() const;

private:
    int m_index = -1;
    QStringList m_list;

    QScopedPointer<Ui::SelectionWidget> m_ui;
};
