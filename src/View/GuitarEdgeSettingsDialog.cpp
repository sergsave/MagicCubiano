#include "GuitarEdgeSettingsDialog.h"
#include "ui_GuitarEdgeSettingsDialog.h"

#include <cassert>
#include <algorithm>

#include <QCheckBox>
#include <set>

namespace {
// http://static1.squarespace.com/static/57a08f8a414fb546e8e35178/587dae1f9f7456b5f7fc5413/59e456f08dd041f0a6c9ed93/1508217399278/Guitar_fret_notes_and_frequency_hz.png?format=1500w

using namespace Music;

const QVector<Tone> tonesForString(const Tone& firstTone)
{
    const int fretsQuantity = 21;
    const auto firstFullOctave = allTonesForOctave(firstTone.octave);
    const int octaveSize = firstFullOctave.size();

    const int firstOctaveOffset = firstFullOctave.indexOf(firstTone);
    const int remainingFrets = fretsQuantity - (octaveSize - firstOctaveOffset);
    const int fullOctavesQuantity = remainingFrets / octaveSize;
    const int lastOctaveOffset = remainingFrets % octaveSize;

    auto ret = firstFullOctave.mid(firstOctaveOffset);

    int octaveNumb = firstTone.octave;
    const int lastOctaveNumb = octaveNumb + fullOctavesQuantity + 1;

    while(++octaveNumb != lastOctaveNumb)
        ret += allTonesForOctave(octaveNumb);

    ret += allTonesForOctave(lastOctaveNumb).mid(0, lastOctaveOffset);

    return ret;
}

static const QVector<QVector<Tone>> g_musicTones
{
    // Start with sixth string
    tonesForString(Tone(Tone::E, 2)),
    tonesForString(Tone(Tone::A, 2)),
    tonesForString(Tone(Tone::D, 3)),
    tonesForString(Tone(Tone::G, 3)),
    tonesForString(Tone(Tone::B, 3)),
    tonesForString(Tone(Tone::E, 4)),
};

struct FretboardPos
{
    FretboardPos(int str = -1, int fret = -1) : string(str), fret(fret) {}
    int string;
    int fret;

    bool isValid() const { return string != -1 && fret != -1; }
};

Tone musicToneFor(const FretboardPos& frPos)
{
    if(!frPos.isValid())
    {
        assert("not valid");
        return {};
    }

    const int string = frPos.string;
    const int fret = frPos.fret;

    if(string < 0 || string >= g_musicTones.size())
    {
        assert("out of range");
        return {};
    }

    const auto tonesForString  = g_musicTones[string];

    if(fret < 0 || fret >= tonesForString.size())
    {
        assert("out of range");
        return {};
    }

    return tonesForString[fret];
}

FretboardPos fretboardPosFor(const Tone& tone, const std::set<int>& stringsNumb)
{
    for(int string: stringsNumb)
    {
        int fret = g_musicTones[string].indexOf(tone);

        if(fret == -1)
            continue;

        return {string, fret};
    }

    return {};
}

FretboardPos findFretboardPos(const Tone& tone, const std::set<int>& strings, int maxFret)
{
    auto copyStrings = strings;

    auto pos = fretboardPosFor(tone, copyStrings);

    if(!pos.isValid())
        return pos;

    if(pos.fret > maxFret)
    {
        copyStrings.erase(pos.string);
        pos = findFretboardPos(tone, copyStrings, maxFret);
    }

    return pos;
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

    for(int i = 0; i != m_stringQuantity; ++i)
    {
        auto w = new StringWidget(m_fretQuantity, this);
        m_strings.append(w);
        layout->addWidget(w);
    }

    setWindowFlags(Qt::Window);
}

void GuitarEdgeSettingsDialog::setIcon(const QPixmap &pm)
{
    m_ui->iconLabel->setPixmap(pm);
}

GuitarEdgeSettingsDialog::~GuitarEdgeSettingsDialog() = default;

Music::Harmony GuitarEdgeSettingsDialog::harmony() const
{
    QVector<StringWidget*> activeStrings;

    std::copy_if(m_strings.begin(), m_strings.end(), std::back_inserter(activeStrings), [](StringWidget const * sw){
        return !sw->isMuted();
    });

    Music::Tones tones;

    std::transform(activeStrings.begin(), activeStrings.end(), std::back_inserter(tones), [this](StringWidget * sw) {
        auto string = m_strings.indexOf(sw);
        auto fret = sw->fretValue();
        return musicToneFor(FretboardPos{string, fret});
    });

    return {tones, m_delayMSec};
}

void GuitarEdgeSettingsDialog::setHarmony(const Harmony& harmony)
{
    std::set<int> vacantStringsNumb;

    for(int strNumb = 0; strNumb != m_stringQuantity; ++strNumb)
        vacantStringsNumb.insert(strNumb);

    QVector<FretboardPos> positions;

    for(const auto &tone : harmony.tones)
    {
        auto pos = findFretboardPos(tone, vacantStringsNumb, m_fretQuantity);

        if(!pos.isValid())
            continue;

        vacantStringsNumb.erase(pos.string);
        positions.append(pos);
    }

    for(auto sw: m_strings)
    {
        sw->setFretValue(0);
        sw->setMuted(true);
    }

    for(const auto& pos: positions)
    {
        m_strings[pos.string]->setMuted(false);
        m_strings[pos.string]->setFretValue(pos.fret);
    }
}
