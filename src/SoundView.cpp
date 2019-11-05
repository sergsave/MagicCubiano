#include "SoundView.h"

#include "Preset/Presets.h"
#include "Model/Model.h"
#include "Sound/SoundGeneratorFactory.h"

namespace {

 SoundGenerator * soundGeneratorFor(Preset::AbstractPreset * preset)
{
    class SoundGeneratorVisitor : public Preset::ConstVisitor
    {
        using T = Instruments::Type;
    public:
        void visit(const Preset::GuitarPreset&)  override { m_type = T::GUITAR; }
        void visit(const Preset::ElectricGuitarPreset&) override { m_type = T::ELECTRIC_GUITAR; }
        void visit(const Preset::PianoPreset&) override { m_type = T::PIANO; }

        SoundGenerator * get() const { return createSoundGenerator(m_type); }

    private:
        T m_type;
    } visitor;

    preset->acceptVisitor(visitor);
    return visitor.get();
}

}

SoundView::SoundView(const Model *model, QObject *parent)
    : QObject(parent),
      m_model(model)
{
    auto activePreset = [this] {
        auto presets = m_model->presets();
        return presets->findPreset(presets->activePreset());
    };

    auto updateGenerator = [activePreset, this] {
        if(m_generator)
        {
            delete m_generator;
            m_generator = nullptr;
        }

        if(auto preset = activePreset())
            m_generator = soundGeneratorFor(preset);
    };

    updateGenerator();
    connect(m_model->presets(), &PresetModel::changed, this, updateGenerator);

    auto play = [this, activePreset] (const CubeEdge& ce) {

        auto preset = activePreset();
        if(!preset || !m_generator)
            return;

        auto vol = m_model->settings()->volume();
        m_generator->play(preset->toHarmony(ce), vol);
    };

    connect(m_model->remoteProtocol(), &GiikerProtocol::cubeEdgeTurned, this, play);
}
