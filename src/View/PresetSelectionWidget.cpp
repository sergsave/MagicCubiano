#include "PresetSelectionWidget.h"
#include "ui_PresetSelectionWidget.h"

#include "Utils.h"

PresetSelectionWidget::PresetSelectionWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::PresetSelectionWidget)
{
    m_ui->setupUi(this);

    m_ui->nameLineEdit->setReadOnly(true);
    m_ui->nameLineEdit->setFrame(false);

    updateState();

    connect(m_ui->prevButton, &QPushButton::clicked, this, [this] {
        changeIndex(--m_index);
    });

    connect(m_ui->nextButton, &QPushButton::clicked, this, [this] {
        changeIndex(++m_index);
    });

    connect(m_ui->openButton, &QAbstractButton::clicked, this, [this] {
        if(!m_presets.isEmpty())
            emit presetEditRequested(currentPreset());
    });

    connect(m_ui->renameButton, &QAbstractButton::toggled, this, [this] (bool checked) {
        m_ui->nameLineEdit->setReadOnly(!checked);
        m_ui->nameLineEdit->setFrame(checked);
        m_ui->openButton->setEnabled(!checked);

        if(checked)
        {
            m_ui->nextButton->setEnabled(false);
            m_ui->prevButton->setEnabled(false);
            return;
        }

        auto oldName = currentPreset();
        auto newName = m_ui->nameLineEdit->text();
        if(newName == oldName)
            return;

        auto inst = m_presets2instruments.value(oldName);
        m_presets2instruments.remove(oldName);
        m_presets2instruments[newName] = inst;

        m_presets[m_index] = m_ui->nameLineEdit->text();

        updateState();

        emit presetRenamed(oldName, newName);
    });

    connect(m_ui->nameLineEdit, &QLineEdit::textChanged, this, [this](const QString& text) {
        if(m_ui->nameLineEdit->isReadOnly())
            return;

        bool isValid = !text.isEmpty() && !m_presets.contains(text);
        m_ui->renameButton->setEnabled(isValid);
        m_ui->nameLineEdit->setStyleSheet(isValid ? "" : "border: 2px solid red");
    });


}

QString PresetSelectionWidget::currentPreset() const
{
    return m_presets[m_index];
}

bool PresetSelectionWidget::remove(const QString& name)
{
    if(m_presets.indexOf(name) == -1)
        return false;

    m_presets.removeFirst();
    changeIndex(m_index);
    return true;
}

bool PresetSelectionWidget::add(const QString& name, const QString& instrument)
{
    if(m_presets.indexOf(name) != -1)
        return false;

    m_presets.append(name);
    m_presets2instruments.insert(name, instrument);

    changeIndex(m_presets.size() - 1);
    return true;
}

void PresetSelectionWidget::changeIndex(int newIdx)
{
    if(m_presets.isEmpty())
        return;

    int bounded = qBound(0, newIdx, m_presets.size() - 1);
    bool changed = m_index != bounded;

    m_index = bounded;

    updateState();

    if(changed)
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
    QString instName;

    if(!m_presets.isEmpty())
    {
        name = currentPreset();
        instName = m_presets2instruments.value(name);
    }

    m_ui->nameLineEdit->setText(name);
    m_ui->typeLabel->setText(instName);
}

PresetSelectionWidget::~PresetSelectionWidget() = default;
