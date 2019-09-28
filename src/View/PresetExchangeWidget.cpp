#include "PresetExchangeWidget.h"

#include <QBoxLayout>
#include <QToolButton>
#include <QLabel>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDebug>

#include <cassert>

namespace {

QString openFileDialog(QFileDialog::AcceptMode mode)
{
    const auto path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    QFileDialog dialog(nullptr, "", path,"Text (*.txt)");
    dialog.setWindowFlags(Qt::Window);
    dialog.setAcceptMode(mode);
    dialog.exec();

    auto selected = dialog.selectedFiles();

    if(selected.isEmpty())
        return {};

    return selected.first();
}

}

PresetExchangeWidget::PresetExchangeWidget(QWidget *parent) : QWidget(parent)
{
    auto layout = new QHBoxLayout(this);

    auto createButton = [layout, this] (const QString& name){
        auto button = new QToolButton(this);
        button->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        button->setText(name);
        layout->addWidget(button);
        return button;
    };

    auto save = createButton("Save Preset");
    auto load = createButton("Load Preset");

    auto status = new QLabel(this);
    layout->addWidget(status);

    connect(save, &QAbstractButton::clicked, this, [this, status] { onSaveClicked(); status->clear(); });
    connect(load, &QAbstractButton::clicked, this, [this, status] { onLoadClicked(); status->clear(); });
    connect(this, &PresetExchangeWidget::presetImportFailed, status, [status] {status->setText("Failed!");});
}

void PresetExchangeWidget::setPresetCompositor(const std::function<Preset ()> &compositor)
{
    m_compositor = compositor;
}

void PresetExchangeWidget::onSaveClicked()
{
    if(!m_compositor)
        return assert(!"not inited");

    auto path = openFileDialog(QFileDialog::AcceptSave);
    auto preset = m_compositor();

    QFile saveFile(path);

    if(!saveFile.open(QFile::WriteOnly))
    {
        qDebug() << "not opened";
        return;
    }

    saveFile.write(presetToJson(preset));
}

void PresetExchangeWidget::onLoadClicked()
{
    if(!m_compositor)
        return assert(!"not inited");

    auto path = openFileDialog(QFileDialog::AcceptOpen);

    QFile loadFile(path);
    if(!loadFile.open(QFile::ReadOnly))
    {
        qDebug() << "not opened";
        return;
    }

    bool ok = false;
    auto preset = presetFromJson(loadFile.readAll(), ok);
    auto name = QFileInfo(loadFile).fileName();

    if(!ok)
    {
        emit presetImportFailed(name);
        return;
    }

    emit presetImported(name, preset);
}
