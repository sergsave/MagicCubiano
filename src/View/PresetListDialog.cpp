#include "PresetListDialog.h"
#include "ui_PresetListDialog.h"

#include <QPushButton>

#include "PresetListElementWidget.h"

PresetListDialog::PresetListDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::PresetListDialog)
{
    m_ui->setupUi(this);

    setWindowFlags(Qt::Window);

    connect(m_ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

PresetListDialog::~PresetListDialog() = default;

void PresetListDialog::setPresetList(const QStringList & presets)
{
    m_displayedPresets = presets;

    auto createWidget = [this] (const QString& name) {
        auto widget = new PresetListElementWidget(this);
        widget->setName(name);
        widget->setNameValidator([this](const QString& name) {
            return isValidName(name);
        });
        return widget;
    };

    for(auto& name: presets)
    {
        auto item = new QListWidgetItem(m_ui->listWidget);
        auto w = createWidget(name);
        item->setSizeHint(w->sizeHint());

        m_ui->listWidget->setItemWidget(item, w);

        connect(w, &PresetListElementWidget::removeClicked, this, [item, name, this] {
            m_ui->listWidget->removeItemWidget(item);
            m_displayedPresets.removeAt(m_ui->listWidget->row(item));
            m_renamedPresets.remove(name);
            m_removedPresets.append(name);
        });

        connect(w, &PresetListElementWidget::nameChanged, this, [this, name, item](const QString& newName) {
            bool isValid = isValidName(newName);

            if(isValid && newName != name)
                m_renamedPresets[name] = newName;

            int idx = m_ui->listWidget->row(item);
            m_displayedPresets[idx] = newName;

            auto okButton = m_ui->buttonBox->button(QDialogButtonBox::StandardButton::Ok);
            okButton->setEnabled(isValid);

            for(int i = 0; i != m_ui->listWidget->count(); ++i)
            {
                auto iteratedItem = m_ui->listWidget->item(i);
                if(iteratedItem != item)
                    m_ui->listWidget->itemWidget(iteratedItem)->setEnabled(isValid);
            }
        });
    }
}

QMap<QString, QString> PresetListDialog::renamedPresets() const
{
    return m_renamedPresets;
}

QStringList PresetListDialog::removedPresets() const
{
    return m_removedPresets;
}

bool PresetListDialog::isValidName(const QString& name) const
{
    return !m_displayedPresets.contains(name);
}
