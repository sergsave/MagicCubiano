#include "SaveLoadHelper.h"

#include <functional>

#include <QFileDialog>
#include <QStandardPaths>

namespace {

QString openFileDialog(const std::function<void(QFileDialog&)>& setup)
{
    const auto path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    QFileDialog dialog(nullptr, "", path, "Json (*.json)");
    dialog.setDefaultSuffix(".json");
    dialog.setWindowFlags(Qt::Window);
    setup(dialog);

    if(dialog.exec() == QDialog::Rejected)
        return {};

    auto selected = dialog.selectedFiles();

    if(selected.isEmpty())
        return {};

    return selected.first();
}

}

SaveLoadHelper::SaveLoadHelper(QObject *parent)
    : QObject(parent)
{
}

SaveLoadHelper::~SaveLoadHelper() = default;

// BUG! Check dialog result
void SaveLoadHelper::startLoading()
{
    auto path = openFileDialog([](QFileDialog & dialog) {
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
    });
    emit loadRequested(path);
}

void SaveLoadHelper::startSaving(const QString &name)
{
    auto path = openFileDialog([name](QFileDialog & dialog) {
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        dialog.selectFile(name + ".json");
    });
    emit saveRequested(name, path);
}
