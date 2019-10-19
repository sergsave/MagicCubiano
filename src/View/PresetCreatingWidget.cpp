#include "PresetCreatingWidget.h"
#include "ui_PresetCreatingWidget.h"

#include <QVBoxLayout>
#include <QButtonGroup>
#include <QPushButton>

#include "src/Instruments/Common.h"
#include "src/Configuration.h"
#include "src/Preset/Storage.h"
#include "Utils.h"

PresetCreatingWidget::PresetCreatingWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::PresetCreatingWidget)
{
    m_ui->setupUi(this);

    auto layout = new QVBoxLayout(m_ui->instrumentButtonsFrame);
    auto buttonGroup = new QButtonGroup(this);

    auto addButton = [this, layout, buttonGroup](Instruments::Type t) {
        auto b = new QPushButton(this);
        b->setText(instrumentName(t));
        layout->addWidget(b);
        buttonGroup->addButton(b, static_cast<int>(t));
        return b;
    };

    for(auto t : Configuration::supportedInstruments())
        addButton(t);

    for(auto t: Configuration::soonInstruments())
        addButton(t)->setEnabled(false);

    connect(buttonGroup, QOverload<int>::of(&QButtonGroup::buttonClicked), this, [this] (int id) {
        auto name = m_ui->nameLineEdit->text();
        emit presetCreated(name, Preset::createPreset(static_cast<Instruments::Type>(id)));
    });
}

void PresetCreatingWidget::setPresetName(const QString &name)
{
    m_ui->nameLineEdit->setText(name);
}

PresetCreatingWidget::~PresetCreatingWidget() = default;

