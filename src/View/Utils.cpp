#include "Utils.h"

#include "src/Preset/Presets.h"

#include <cassert>
#include <QMap>

QString instrumentName(Instruments::Type type)
{
    using T = Instruments::Type;

    switch (type)
    {
    case T::GUITAR:
        return "Guitar";
    case T::ELECTRIC_GUITAR:
        return "Electric guitar";
    case T::PIANO:
        return "Piano";
    default:
        assert(!"unreal");
        return {};
    }
}

QString instrumentName(Preset::AbstractPreset *preset)
{
    class InstrumentTypeFetchVisitor : public Preset::ConstVisitor
    {
        using T = Instruments::Type;
    public:
        void visit(const Preset::GuitarPreset&) override { m_type = T::GUITAR; }
        void visit(const Preset::ElectricGuitarPreset&) override { m_type = T::ELECTRIC_GUITAR; }
        void visit(const Preset::PianoPreset&) override { m_type = T::PIANO; }

        Instruments::Type type() const { return m_type; }

    private:
        T m_type;
    };

    InstrumentTypeFetchVisitor visitor;
    preset->acceptVisitor(visitor);
    return instrumentName(visitor.type());
}

EdgeAppearance appearanceFor(const CubeEdge &edge)
{
    using Col = CubeEdge::Color;
    const QMap<Col, QColor> colors
    {
        { Col::GREEN, Qt::darkGreen },
        { Col::BLUE, Qt::darkBlue },
        { Col::RED, "#FFB6C1" }, // Light pink Giiker V1 peculiarity
        { Col::ORANGE, "#FF4500"},  // Orangered Giiker V1 peculiarity
        { Col::YELLOW, Qt::yellow},
        { Col::WHITE, Qt::white}
    };

    using Rot = CubeEdge::Rotation;
    const QMap<Rot, QString> iconPaths
    {
        { Rot::CLOCKWIZE, ":/images/clockwize.png" },
        { Rot::ANTICLOCKWIZE, ":/images/anticlockwize.png" }
    };

    return { colors.value(edge.color, Qt::black), iconPaths.value(edge.rotation) };
}
