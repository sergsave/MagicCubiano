#include "SaveLoadHelper.h"

#include <functional>

#include <QFileDialog>
#include <QStandardPaths>

#include "src/Preset/Storage.h"
#include "src/Preset/SaveLoader.h"

namespace {

QString openFileDialog(const std::function<void(QFileDialog&)>& setup)
{
    const auto path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    QFileDialog dialog(nullptr, "", path, "Json (*.json)");
    dialog.setDefaultSuffix(".json");
    dialog.setWindowFlags(Qt::Window);
    setup(dialog);
    dialog.exec();

    auto selected = dialog.selectedFiles();

    if(selected.isEmpty())
        return {};

    return selected.first();
}

}

SaveLoadHelper::SaveLoadHelper(Preset::Storage * storage, QObject *parent)
    : QObject(parent),
      m_storage(storage),
      m_saveLoader(new Preset::SaveLoader)
{
}

SaveLoadHelper::~SaveLoadHelper() = default;

void SaveLoadHelper::load()
{
    auto path = openFileDialog([](QFileDialog & dialog) {
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
    });

    auto preset = m_saveLoader->load(path);
    auto name = QFileInfo(path).baseName();

    if(!preset)
    {
        emit presetLoadFailed(name);
        return;
    }

    auto vacName = Preset::generateVacantName(*m_storage, name); // TODO: or failed
    m_storage->addPreset(vacName, preset);
    emit presetLoaded(vacName);
}

void SaveLoadHelper::save(const QString &name) const
{
    auto path = openFileDialog([name](QFileDialog & dialog) {
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.selectFile(name + ".json");
    });

    auto preset = m_storage->findPreset(name);
    m_saveLoader->save(path, preset);
}
