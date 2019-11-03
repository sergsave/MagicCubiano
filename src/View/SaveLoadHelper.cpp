#include "SaveLoadHelper.h"

#include <functional>

#include <QFileDialog>
#include <QStandardPaths>

namespace {

const QString g_jsonExt = ".json";

void setJsonFilter(QFileDialog& dialog)
{
    dialog.setNameFilter(QString("Json (*%1)").arg(g_jsonExt));
    dialog.setDefaultSuffix(g_jsonExt);
}

QString openFileDialog(const std::function<void(QFileDialog&)>& setup)
{
    const auto path = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

    QFileDialog dialog(nullptr, "", path);
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

void SaveLoadHelper::choosePathForLoading()
{
    auto path = openFileDialog([](QFileDialog & dialog) {
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        setJsonFilter(dialog);
    });
    emit loadRequested(path);
}

void SaveLoadHelper::choosePathForLoadingAll()
{
    auto dir = openFileDialog([](QFileDialog & dialog) {
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(QFileDialog::DirectoryOnly);
    });

    emit loadAllRequested(dir);
}

void SaveLoadHelper::choosePathForSaving(const QString &name)
{
    auto path = openFileDialog([name](QFileDialog & dialog) {
        dialog.setAcceptMode(QFileDialog::AcceptSave);
        setJsonFilter(dialog);
        dialog.selectFile(name + g_jsonExt);
    });
    emit saveRequested(name, path);
}
