#include "PresetListElementWidget.h"

#include <QHBoxLayout>
#include <QToolButton>
#include <QLineEdit>

PresetListElementWidget::PresetListElementWidget(QWidget *parent) : QWidget(parent)
{
    auto layout = new QHBoxLayout(this);
    m_removeButton = new QToolButton(this);
    m_lineEdit = new QLineEdit(this);

    m_removeButton->setText("Remove");

    layout->setStretch(0, 1);
    layout->addWidget(m_lineEdit);
    layout->addWidget(m_removeButton);

    connect(m_removeButton, &QToolButton::clicked, this, &PresetListElementWidget::removeClicked);
    connect(m_lineEdit, &QLineEdit::textChanged, this, [this] {
        auto name = m_lineEdit->text();

        if(m_validator)
        {
            bool res = m_validator(name);
            m_lineEdit->setStyleSheet(res ? "" : "border: 2px solid red");
            m_removeButton->setEnabled(res);
        }

        emit nameChanged(name);
    });
}

void PresetListElementWidget::setName(const QString& name)
{
    m_lineEdit->setText(name);
}

void PresetListElementWidget::setNameValidator(const std::function<bool(const QString&)>& validator)
{
    m_validator = validator;
}
