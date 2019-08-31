#include "SelectionWidget.h"
#include "ui_SelectionWidget.h"

SelectionWidget::SelectionWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::SelectionWidget)
{
    m_ui->setupUi(this);

    connect(m_ui->leftButton, &QAbstractButton::clicked, this, &SelectionWidget::decrease);
    connect(m_ui->rightButton, &QAbstractButton::clicked, this, &SelectionWidget::increase);

    onIndexUpdated(m_index);
}

SelectionWidget::~SelectionWidget() = default;

void SelectionWidget::setTitle(const QString & str)
{
    m_ui->titleLabel->setText(str);
}

QString SelectionWidget::title() const
{
    return m_ui->titleLabel->text();
}

void SelectionWidget::setValues(const QStringList & list)
{
    m_list = list;

    auto idx = list.empty() ? -1 : 0;
    onIndexUpdated(idx);
    m_index = idx;
}

QStringList SelectionWidget::values() const
{
    return m_list;
}

int SelectionWidget::index() const
{
    return m_index;
}

void SelectionWidget::increase()
{
    auto idx = m_index;
    if(idx++ == maxIndex())
        idx = 0;

    onIndexUpdated(idx);
    m_index = idx;
}

void SelectionWidget::decrease()
{
    auto idx = m_index;
    if(idx-- == 0)
        idx = maxIndex();

    onIndexUpdated(idx);
    m_index = idx;
}

void SelectionWidget::onIndexUpdated(int idx)
{
    if(idx != m_index)
        emit indexChanged(idx);

    m_ui->leftButton->setEnabled(idx > 0);
    m_ui->rightButton->setEnabled(idx < maxIndex());

    m_ui->valueLabel->setText(m_list.value(idx));
}

int SelectionWidget::maxIndex() const
{
    return m_list.size() - 1;
}
