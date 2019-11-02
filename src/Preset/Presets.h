#pragma once

#include <QString>
#include <QMap>

#include "Visitor.h"
#include "src/CubeEdge.h"

namespace Preset {

class AbstractPreset
{
public:
    virtual ~AbstractPreset() {}
    virtual void acceptVisitor(Visitor& v) = 0;
    virtual void acceptVisitor(ConstVisitor& v) const = 0;
    virtual Music::Harmony toHarmony(const CubeEdge&) const = 0;

    virtual void setMinDurationMs(const CubeEdge&, int ms) = 0;
    virtual int minDurationMs(const CubeEdge&) const = 0;

    class Backup
    {
    public:
        virtual ~Backup() {}
        virtual void restore() = 0;
    };

    virtual Backup * createBackup() = 0;
};

template <class InstrumentTag>
class TPreset : public AbstractPreset
{
public:
    using Notations = QList<typename InstrumentTag::Notation>;

    struct Unit
    {
        int minDurationMSec = 0;
        Notations notations;
    };

    using Data = QMap<CubeEdge::Rotation, QMap<CubeEdge::Color, Unit>>;

    void acceptVisitor(Visitor& v) override { v.visit(*this); }
    void acceptVisitor(ConstVisitor& cv) const override { cv.visit(*this); }

    Music::Harmony toHarmony(const CubeEdge& ce) const override
    {
        Music::Harmony ret;
        auto unit = m_data[ce.rotation][ce.color];

        ret.minToneDurationMSec = unit.minDurationMSec;
        for(auto notation: unit.notations)
            ret.tones.append(Desc::toneFor(notation));
        return ret;
    }

    void setMinDurationMs(const CubeEdge& ce, int ms) override
    {
        m_data[ce.rotation][ce.color].minDurationMSec = ms;
    }

    int minDurationMs(const CubeEdge& ce) const override
    {
        return m_data[ce.rotation][ce.color].minDurationMSec;
    }

    void setNotations(const CubeEdge& ce, const Notations& notations)
    {
        m_data[ce.rotation][ce.color].notations = notations;
    }

    Notations notations(const CubeEdge& ce) const
    {
        return m_data[ce.rotation][ce.color].notations;
    }

    void setData(const Data& data)
    {
        m_data = data;
    }

    Data data() const
    {
        return m_data;
    }

    Backup * createBackup() override
    {
        return new ConcreteBackup(this);
    }

private:

    class ConcreteBackup: public Backup
    {
    public:
        ConcreteBackup(TPreset<InstrumentTag> * preset) : m_preset(preset)
        {
            m_data = m_preset->data();
        }

        void restore() override
        {
            m_preset->setData(m_data);
        }
    private:
        typename TPreset<InstrumentTag>::Data m_data;
        TPreset<InstrumentTag> * m_preset;
    };

    using Desc = Instruments::Description<InstrumentTag>;
    Data m_data;
};

}
