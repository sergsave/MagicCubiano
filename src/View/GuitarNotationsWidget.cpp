#include "GuitarNotationsWidget.h"

#include <cassert>
#include <QHBoxLayout>

#include "GuitarStringWidget.h"

GuitarNotationsWidget::GuitarNotationsWidget(const Instruments::GuitarNotation& min,
                                             const Instruments::GuitarNotation& max,
                                             QWidget *parent)
    : QWidget(parent)
{
    auto layout = new QHBoxLayout(this);

    for(int i = min.string; i <= max.string; ++i)
    {
        auto stringW = new GuitarStringWidget(i, min.fret, max.fret, this);
        layout->addWidget(stringW);

        connect(stringW, &GuitarStringWidget::fretValueChanged, this, [stringW, this](int val) {
            onFretChanged(stringW->number(), val);
        });

        connect(stringW, &GuitarStringWidget::muteChaged, this, [stringW, this](bool muted) {
            onMuteChanged(stringW->number(), muted);
        });

        m_widgets.append(stringW);
    }
}

void GuitarNotationsWidget::setNotations(const GuitarNotationsWidget::Notations &notations)
{
    auto prevNotations = this->notations();

    for(auto w: m_widgets)
        w->reset();

    for(auto n: notations)
    {
        auto widget = stringWidget(n.string);
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

    for(auto widget: m_widgets)
    {
        if(widget->isMuted())
            continue;

        ret.push_back({widget->number(), widget->fretValue()});
    }

    return ret;
}

GuitarStringWidget * GuitarNotationsWidget::stringWidget(int string)
{
    return m_widgets.value(string, nullptr);
}

void GuitarNotationsWidget::onFretChanged(int string, int fret)
{
    auto widget = stringWidget(string);
    if(!widget)
        return;

    widget->setFretValue(fret);
    emit notationsChanged(notations());
}

void GuitarNotationsWidget::onMuteChanged(int string, bool mute)
{
    auto widget = stringWidget(string);
    if(!widget)
        return;

    widget->setMuted(mute);
    emit notationsChanged(notations());
}

GuitarNotationsWidget::~GuitarNotationsWidget() = default;
