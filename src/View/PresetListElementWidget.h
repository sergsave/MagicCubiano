#pragma once

#include <functional>
#include <QWidget>

class QToolButton;
class QLineEdit;

class PresetListElementWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PresetListElementWidget(QWidget *parent = nullptr);

    void setName(const QString&);
    void setNameValidator(const std::function<bool(const QString&)>&);

signals:
    void removeClicked();
    void nameChanged(const QString&);

private:
    QToolButton * m_removeButton = nullptr;
    QLineEdit * m_lineEdit = nullptr;
    std::function<bool(const QString&)> m_validator;
};
