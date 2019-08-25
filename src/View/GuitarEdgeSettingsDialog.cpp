#include "GuitarEdgeSettingsDialog.h"
#include "ui_GuitarEdgeSettingsDialog.h"

#include <cassert>
#include <algorithm>

#include <QCheckBox>
#include <QDebug>

namespace {
// http://static1.squarespace.com/static/57a08f8a414fb546e8e35178/587dae1f9f7456b5f7fc5413/59e456f08dd041f0a6c9ed93/1508217399278/Guitar_fret_notes_and_frequency_hz.png?format=1500w

using namespace Music;
using T = Tone;

const QVector<Tone> fullOctave(int n)
{
    return
    {
        Tone(T::C, T::None, n), Tone(T::C, T::Sharp, n),
        Tone(T::D, T::None, n), Tone(T::D, T::Sharp, n),
        Tone(T::E, T::None, n),
        Tone(T::F, T::None, n), Tone(T::F, T::Sharp, n),
        Tone(T::G, T::None, n), Tone(T::G, T::Sharp, n),
        Tone(T::A, T::None, n), Tone(T::A, T::Sharp, n),
        Tone(T::B, T::None, n)
    };
}

const QVector<Tone> tonesForString(const Tone& firstTone)
{
    const int fretsQuantity = 21;
    const auto firstFullOctave = fullOctave(firstTone.octave);
    const int octaveSize = firstFullOctave.size();

    const int firstOctaveOffset = firstFullOctave.indexOf(firstTone);
    const int remainingFrets = fretsQuantity - (octaveSize - firstOctaveOffset);
    const int fullOctavesQuantity = remainingFrets / octaveSize;
    const int lastOctaveOffset = remainingFrets % octaveSize;

    QVector<Tone> ret = firstFullOctave.mid(firstOctaveOffset);

    int octaveNumb = firstTone.octave;
    const int lastOctaveNumb = octaveNumb + fullOctavesQuantity + 1;

    while(++octaveNumb != lastOctaveNumb)
        ret += fullOctave(octaveNumb);

    ret += fullOctave(lastOctaveNumb).mid(0, lastOctaveOffset);

    return ret;
}

Music::Tone musicToneFor(int string, int fret)
{
    static const QVector<QVector<Tone>> musicTones
    {
        // Start with first string
        tonesForString(Tone(T::E, T::None, 2)),
        tonesForString(Tone(T::B, T::None, 1)),
        tonesForString(Tone(T::G, T::None, 1)),
        tonesForString(Tone(T::D, T::None, 1)),
        tonesForString(Tone(T::A, T::None, 0)),
        tonesForString(Tone(T::E, T::None, 0)),
    };

    if(string < 0 || string >= musicTones.size())
    {
        assert("out of range");
        return {};
    }

    const auto tonesForString  = musicTones[string];

    if(fret < 0 || fret >= tonesForString.size())
    {
        assert("out of range");
        return {};
    }

    return tonesForString[fret];
}

}

class StringWidget: public QWidget
{
public:
    StringWidget(int fretQuantity, QWidget * p):
        QWidget(p)
    {
        auto layout = new QVBoxLayout(this);

        m_slider = new QSlider(this);
        m_checkBox = new QCheckBox(this);
        auto label = new QLabel(this);

        m_slider->setMaximum(fretQuantity);
        m_checkBox->setText({});
        label->setText("0");

        layout->addWidget(m_slider);
        layout->addWidget(label);
        layout->addWidget(m_checkBox);

        setMuted(true);

        connect(m_slider, &QSlider::valueChanged, label, [label] (int val) { label->setText(QString::number(val));} );
        connect(m_slider, &QSlider::valueChanged, m_checkBox, [this] { m_checkBox->setChecked(true);} );
    }

    bool isMuted() const
    {
        return !m_checkBox->isChecked();
    }

    void setMuted(bool st)
    {
        m_checkBox->setChecked(!st);
    }

    int fretValue() const
    {
        return m_slider->value();
    }

    void setFretValue(int val)
    {
        m_slider->setValue(val);
    }

private:
    QCheckBox * m_checkBox = nullptr;
    QSlider * m_slider = nullptr;
};

GuitarEdgeSettingsDialog::GuitarEdgeSettingsDialog(QWidget *parent) :
    EdgeSettingsDialog(parent),
    m_ui(new Ui::GuitarEdgeSettingsDialog)
{
    m_ui->setupUi(this);

    auto layout = new QHBoxLayout(m_ui->frame);
    layout->setDirection(QBoxLayout::RightToLeft);

    for(int i = 0; i != m_stringQuantity; ++i)
    {
        auto w = new StringWidget(m_fretQuantity, this);
        m_strings.append(w);
        layout->addWidget(w);
    }

    m_strings.last()->setMuted(false);
}

GuitarEdgeSettingsDialog::~GuitarEdgeSettingsDialog() = default;

Music::Harmony GuitarEdgeSettingsDialog::harmony() const
{
    QVector<StringWidget*> activeStrings;

    // Reverse order. Sound of last string is first.
    std::copy_if(m_strings.begin(), m_strings.end(), std::front_inserter(activeStrings), [](StringWidget * sw){
        return !sw->isMuted();
    });

    Music::Harmony::Tones tones;

    std::transform(activeStrings.begin(), activeStrings.end(), std::back_inserter(tones), [this](StringWidget * sw) {
        auto string = m_strings.indexOf(sw);
        auto fret = sw->fretValue();
        return musicToneFor(string, fret);
    });

    return { tones, m_delayMSec };
}
