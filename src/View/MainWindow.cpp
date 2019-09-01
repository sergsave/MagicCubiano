#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "ConnectionDialog.h"

#include <QScopedPointer>
#include <QDesktopWidget>

#include <cassert>

MainWindow::MainWindow(QWidget *parent):
    QWidget(parent),
    m_ui(new Ui::MainWindow)
{
    m_ui->setupUi(this);

    createSettingsFactory();
    createEdgeWidgets();

    QRect scr = QApplication::desktop()->screenGeometry();
    move( scr.center() - rect().center() );

    connect(m_ui->syncButton, &QAbstractButton::clicked, this, &MainWindow::synchronizeEdgesRotation);
    connect(m_ui->resetButton, &QAbstractButton::clicked, this, &MainWindow::setDefaultHarmonies);
    connect(m_ui->settingsButton, &QAbstractButton::clicked, this, &MainWindow::enterGlobalSettings);
    connect(m_ui->instrumentsWidget, &InstrumentSelectionWidget::instrumentTypeChanged,
            this, &MainWindow::onInstrumentTypeChanged);

    initPresets();
}

MainWindow::~MainWindow()
{
    for(auto w: edgeWidgets()) w->setSettingsFactory(nullptr);
}

void MainWindow::start()
{
    m_dialog = new ConnectionDialog(this);

    connect(m_dialog, &ConnectionDialog::connectAnyRequested, this, &MainWindow::connectAnyRequested);
    connect(m_dialog, &ConnectionDialog::connectByAddressRequested, this, &MainWindow::connectByAddressRequested);
    m_dialog->exec();

    show();
}

Music::Harmony MainWindow::harmonyFor(const CubeEdge & edge) const
{
    auto edgeWidget = m_color2edges.value(edge.color, nullptr);

    if(!edgeWidget)
        return {};

    auto harmony = edgeWidget->harmony(edge.rotation);
    harmony.delayMSec = harmonyDelayMsec();

    return harmony;
}

void MainWindow::highlightEdge(CubeEdge::Color color)
{
    if(auto edge = m_color2edges.value(color, nullptr))
        edge->indicate();
}

Music::Instrument MainWindow::instrumentType() const
{
    return m_ui->instrumentsWidget->instrumentType();
}

void MainWindow::connected()
{
    if(m_dialog) m_dialog->connected();
}

void MainWindow::connectionFailed()
{
    if(m_dialog) m_dialog->connectionFailed();
}

void MainWindow::createEdgeWidgets()
{
    using Col = CubeEdge::Color;

    assert(m_color2edges.empty());

    auto layout = m_ui->edgesFrame->layout();
    if(!layout)
        layout = new QVBoxLayout(m_ui->edgesFrame);

    auto addWidget = [layout, this] (CubeEdge::Color col) {
        auto w = new EdgeWidget(col, m_settingsFactory.data(), this);
        layout->addWidget(w);

        m_color2edges[col] = w;
    };

    addWidget(Col::YELLOW);
    addWidget(Col::ORANGE);
    addWidget(Col::RED);
    addWidget(Col::GREEN);
    addWidget(Col::WHITE);
    addWidget(Col::BLUE);

    setDefaultHarmonies();
}

void MainWindow::onInstrumentTypeChanged(Music::Instrument ins)
{
    updateSettingsFactory();
    emit instrumentTypeChanged(ins);
}

void MainWindow::onPresetChanged(const PresetSelectionWidget::NamedPreset& preset)
{
    auto setupEdgeWidgets = [this, &preset](CubeEdge::Rotation rot){
        auto ews = edgeWidgets();
        auto colors = preset.second[rot];

        for(auto key: colors.keys())
            ews[key]->setHarmony(colors.value(key), rot);
    };

    setupEdgeWidgets(CubeEdge::CLOCKWIZE);
    setupEdgeWidgets(CubeEdge::ANTICLOCKWIZE);
}

void MainWindow::createSettingsFactory()
{
    auto type = m_ui->instrumentsWidget->instrumentType();
    m_settingsFactory.reset(EdgeSettingsFactory::createInstance(type));
}

void MainWindow::updateSettingsFactory()
{
    createSettingsFactory();

    for(auto ew: edgeWidgets())
        ew->setSettingsFactory(m_settingsFactory.data());
}

QList<EdgeWidget *> MainWindow::edgeWidgets()
{
    assert(!m_color2edges.empty());
    return m_color2edges.values();
}

void MainWindow::setAllDirectionHarmony(EdgeWidget *ew, const Music::Harmony& harm)
{
    ew->setHarmony(harm, CubeEdge::Rotation::ANTICLOCKWIZE);
    ew->setHarmony(harm, CubeEdge::Rotation::CLOCKWIZE);
}

void MainWindow::synchronizeEdgesRotation()
{
    for(auto ew: edgeWidgets())
    {
        if(!ew) continue;
        ew->indicate();

        auto currHarmony = ew->harmony(ew->rotateDirection());
        setAllDirectionHarmony(ew, currHarmony);
    }
}

void MainWindow::setDefaultHarmonies()
{
    for(auto ew: edgeWidgets())
    {
        if(!ew) continue;
        ew->indicate();

        using namespace Music;

        auto tone = Tone(Tone::E, 2);
        auto delay = harmonyDelayMsec();
        auto harmony = Harmony({tone}, delay);

        setAllDirectionHarmony(ew, harmony);
    }
}

int MainWindow::harmonyDelayMsec() const
{
    return m_globalSettings.delayMSec;
}

void MainWindow::enterGlobalSettings()
{
   SettingsDialog dialog(m_globalSettings);
   if(dialog.exec() == QDialog::Accepted)
       m_globalSettings = dialog.settings();
}

void MainWindow::initPresets()
{
    auto exchangeW = m_ui->presetExchangeWidget;
    auto selectionW = m_ui->presetSelectionWidget;

    exchangeW->setPresetCompositor([this]{
        Preset preset;

        for(auto w: edgeWidgets())
        {
            auto color = w->edgeColor();
            preset[CubeEdge::CLOCKWIZE][color] = w->harmony(CubeEdge::CLOCKWIZE);
            preset[CubeEdge::ANTICLOCKWIZE][color] = w->harmony(CubeEdge::ANTICLOCKWIZE);
        }

        return preset;
    });

    connect(exchangeW, &PresetExchangeWidget::presetImported, selectionW, [selectionW] (auto name, auto preset) {
        selectionW->addPreset({name, preset});
    });

    connect(selectionW, &PresetSelectionWidget::presetChanged, this, &MainWindow::onPresetChanged);
}
