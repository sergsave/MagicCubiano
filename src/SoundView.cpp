#include "SoundView.h"

// SoundGenerator * soundGeneratorFor(Preset::AbstractPreset * preset)
//{
//    class SoundGeneratorVisitor : public Preset::ConstVisitor
//    {
//        using T = Instruments::Type;
//    public:
//        void visit(const Preset::GuitarPreset&)  override { m_type = T::GUITAR; }
//        void visit(const Preset::ElectricGuitarPreset&) override { m_type = T::ELECTRIC_GUITAR; }
//        void visit(const Preset::PianoPreset&) override { m_type = T::PIANO; }

//        SoundGenerator * get() const { return createSoundGenerator(m_type); }

//    private:
//        T m_type;
//    } visitor;

//    preset->acceptVisitor(visitor);
//    return visitor.get();
//}

/*
    QObject::connect(&window, &MainWindow::connectAnyRequested, &protocol,
        [&protocol]() { protocol.connectToCube();});
    QObject::connect(&window, &MainWindow::connectByAddressRequested, &protocol,
        [&protocol](auto addr) { protocol.connectToCube(addr);});

    QObject::connect(&protocol, &GiikerProtocol::cubeConnected,
        &window, &MainWindow::connected);
    QObject::connect(&protocol, &GiikerProtocol::cubeConnectionFailed,
        &window, &MainWindow::connectionFailed);

    auto updateGenerator = [&soundGenerator] (Instruments::Type inst) {
        soundGenerator.reset(createSoundGenerator(inst));
    };

    updateGenerator(window.instrumentType());
    QObject::connect(&window, &MainWindow::instrumentTypeChanged, updateGenerator);

    QObject::connect(&protocol, &GiikerProtocol::cubeEdgeTurned, &window,
        [&soundGenerator, &window] (const CubeEdge& info) {

        window.onEdgeTurned(info.color);

        if(soundGenerator)
            soundGenerator->play(window.harmonyFor(info), window.volume());
    });
*/

SoundView::SoundView(const Model *model, QObject *parent)
    : QObject(parent),
      m_model(model)
{

}
