#include "PresetSelectionWidget.h"
#include "ui_PresetSelectionWidget.h"

#include "Utils.h"

PresetSelectionWidget::PresetSelectionWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::PresetSelectionWidget)
{
    m_ui->setupUi(this);

    updateState();

    connect(m_ui->prevButton, &QPushButton::clicked, this, [this] {
        changeIndex(m_index - 1);
    });

    connect(m_ui->nextButton, &QPushButton::clicked, this, [this] {
        changeIndex(m_index + 1);
    });

    connect(m_ui->openButton, &QAbstractButton::clicked, this, [this] {
        if(!m_presets.isEmpty())
            emit presetOpenRequested(selectedPreset());
    });
}

void PresetSelectionWidget::setPresets(const QStringList &presets)
{
    m_presets = presets;
    changeIndex(0, true);
}

QStringList PresetSelectionWidget::presets() const
{
    return m_presets;
}

void PresetSelectionWidget::setPresetAdditionalInfo(const QString &preset, const QString &info)
{
    m_presets2info[preset] = info;
    updateState();
}

QString PresetSelectionWidget::presetAdditionalInfo(const QString &preset) const
{
    return m_presets2info.value(preset);
}

QString PresetSelectionWidget::selectedPreset() const
{
    return m_presets[m_index];
}

void PresetSelectionWidget::setSelectedPreset(const QString & preset)
{
    if(!m_presets.contains(preset))
        return;

    changeIndex(m_presets.indexOf(preset));
}

void PresetSelectionWidget::changeIndex(int newIdx, bool forceChanged)
{
    auto max = m_presets.isEmpty() ? 0 : m_presets.size() - 1;

    int bounded = qBound(0, newIdx, max);
    bool changed = m_index != bounded;

    m_index = bounded;

    updateState();

    if(m_presets.isEmpty())
        return;

    if(changed || forceChanged)
        emit presetSelected(m_presets[m_index]);
}

void PresetSelectionWidget::updateDirectionButtonsState()
{
    bool prevEnabled = false;
    bool nextEnabled = false;

    if(!m_presets.isEmpty())
    {
        prevEnabled = m_index != 0;
        nextEnabled = m_index != (m_presets.size() - 1);
    }

    m_ui->prevButton->setEnabled(prevEnabled);
    m_ui->nextButton->setEnabled(nextEnabled);
}

void PresetSelectionWidget::updateState()
{
    updateDirectionButtonsState();

    QString name;
    QString info;

    if(!m_presets.isEmpty())
    {
        name = selectedPreset();
        info = m_presets2info.value(name);
    }

    m_ui->nameLabel->setText(name);
    m_ui->infoLabel->setText(info);
}

PresetSelectionWidget::~PresetSelectionWidget() = default;
