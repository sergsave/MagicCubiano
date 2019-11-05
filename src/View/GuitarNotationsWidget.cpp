#include "GuitarNotationsWidget.h"

#include <cassert>
#include <QHBoxLayout>
#include <QLabel>

#include "GuitarStringWidget.h"

GuitarNotationsWidget::GuitarNotationsWidget(const Instruments::GuitarNotation& min,
                                             const Instruments::GuitarNotation& max,
                                             QWidget *parent)
    : QWidget(parent)
{
    auto title = new QLabel(this);
    title->setText("Select fret value for each guitar string");
    auto vLayout = new QVBoxLayout(this);
    vLayout->addWidget(title);

    auto hLayout = new QHBoxLayout();
    hLayout->setContentsMargins({});
    hLayout->setSpacing(0);

    vLayout->addItem(hLayout);

    for(int i = min.string; i >= max.string; --i)
    {
        auto stringW = new GuitarStringWidget(i, min.fret, max.fret, this);
        hLayout->addWidget(stringW);

        connect(stringW, &GuitarStringWidget::fretValueChanged, this, [this]{
            emit notationsChanged(notations());
        });

        connect(stringW, &GuitarStringWidget::muteChaged, this, [this] {
            emit notationsChanged(notations());
        });

        m_strings2widgets.insert(i, stringW);
    }
}

void GuitarNotationsWidget::setNotations(const GuitarNotationsWidget::Notations &notations)
{
    auto prevNotations = this->notations();

    for(auto w: m_strings2widgets.values())
        w->reset();

    for(auto n: notations)
    {
        auto widget = m_strings2widgets.value(n.string, nullptr);
        if(!widget)
        {
            assert(!"unreal");
            break;
        }

        widget->setMuted(false);
        widget->setFretValue(n.fret);
    }

    if(prevNotations != notations)
        emit notationsChanged(notations);
}

GuitarNotationsWidget::Notations GuitarNotationsWidget::notations()
{
    Notations ret;

    QMapIterator<int, GuitarStringWidget*> it(m_strings2widgets);

    it.toBack();
    while(it.hasPrevious())
    {
        it.previous();

        auto widget = it.value();
        if(widget->isMuted())
            continue;

        ret.push_back({it.key(), widget->fretValue()});
    }

    return ret;
}

GuitarNotationsWidget::~GuitarNotationsWidget() = default;
