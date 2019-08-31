#include "EdgeSettingsFactory.h"

#include "src/Configuration.h"
#include <cassert>

class GuitarEdgeSettingsFactory : public EdgeSettingsFactory
{
public:
    EdgeSettingsDialog * create(QWidget * parent) const override
    {
        auto interval = Configuration::intervalFor(Music::Instrument::GUITAR);
        return new GuitarEdgeSettingsDialog(interval, parent);
    }
};

EdgeSettingsFactory * EdgeSettingsFactory::createInstance(Music::Instrument ins)
{
    using Ins = Music::Instrument;

    switch (ins)
    {
    case Ins::GUITAR:
    case Ins::DISTORTION_GUITAR:
        return new GuitarEdgeSettingsFactory;
    default:
        assert(!"unsupported");
        return nullptr;
    }
}
