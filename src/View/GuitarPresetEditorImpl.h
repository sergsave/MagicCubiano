#pragma once

#include <functional>
#include <algorithm>

#include "src/Preset/Presets.h"

class IGuitarPresetEditor
{
public:
    virtual ~IGuitarPresetEditor() {}

    virtual Instruments::GuitarNotation min() const = 0;
    virtual Instruments::GuitarNotation max() const = 0;

    using String2Fret = QMap<int, int>;
    virtual String2Fret stateFor(const CubeEdge&) const = 0;

    virtual void onFretChanged(const CubeEdge&, int string, int fret) = 0;
    virtual void onMuteChanged(const CubeEdge&, int string, bool mute) = 0;

    virtual void resetData() = 0;
    virtual void syncDataByClockwize() = 0;
    virtual void syncDataByAnticlockwize() = 0;
};

template <typename GuitarType>
class GuitarPresetEditorImpl : public IGuitarPresetEditor
{
public:
    GuitarPresetEditorImpl(Preset::TPreset<GuitarType> * preset)
        :m_preset(preset)
    {}

    Instruments::GuitarNotation min() const override
    {
        return Instruments::Description<GuitarType>::min();
    }

    Instruments::GuitarNotation max() const override
    {
        return Instruments::Description<GuitarType>::max();
    }

    String2Fret stateFor(const CubeEdge& ce) const override
    {
        String2Fret ret;

        const auto notations = notationsFrom(ce);

        for(auto n: notations)
            ret.insert(n.string, n.fret);

        return ret;
    }

    void onFretChanged(const CubeEdge& ce, int string, int fret) override
    {
        changeNotations(ce, [string, fret, this] (Notations& notations){

        auto it = findNotation(notations, string);
        if(it == notations.end())
            return;

        it->fret = fret;
        });
    }

    void onMuteChanged(const CubeEdge& ce, int string, bool mute) override
    {
        changeNotations(ce, [string, mute, this] (Notations& notations){

        auto it = findNotation(notations, string);

        if(mute)
        {
            if(it != notations.end())
                notations.erase(it);
            return;
        }

        if(it != notations.end())
            return;

        Instruments::GuitarNotation notation;
        notation.string = string;
        notations.append(notation);

        });
    }

    void resetData() override
    {
        for(auto col: CubeEdge::allColors())
        {
            for(auto rot: CubeEdge::allRotations())
                changeNotations({col, rot}, [](Notations& notations){ notations.clear(); });
        }
    }

    void syncDataByClockwize() override
    {
        syncDataByRotation(CubeEdge::CLOCKWIZE);
    }

    void syncDataByAnticlockwize() override
    {
        syncDataByRotation(CubeEdge::ANTICLOCKWIZE);
    }

private:
    using Notations = QList<Instruments::GuitarNotation>;

    Notations notationsFrom(const CubeEdge& ce) const
    {
        auto copy = m_preset->data();
        return refNotationsFrom(copy, ce);
    }

    Notations& refNotationsFrom(auto& presetData, const CubeEdge& ce) const
    {
        return presetData[ce.rotation][ce.color].notations;
    }

    void changeNotations(const CubeEdge& ce, const std::function<void(Notations&)>& changeFunc)
    {
        auto data = m_preset->data();
        auto &notations = refNotationsFrom(data, ce);

        changeFunc(notations);
        m_preset->setData(data);
    }

    Notations::iterator findNotation(Notations& notations, int string) const
    {
        return std::find_if(notations.begin(), notations.end(), [string] (auto& notation) {
            return notation.string == string;
        });
    }

    void syncDataByRotation(CubeEdge::Rotation rotation)
    {
        for(auto col: CubeEdge::allColors())
        {
            auto sync = notationsFrom({col, rotation});
            for(auto rot: CubeEdge::allRotations())
                changeNotations({col, rot}, [&sync, this](Notations& notations){ notations = sync; });
        }
    }

private:
    Preset::TPreset<GuitarType> * m_preset;
};
