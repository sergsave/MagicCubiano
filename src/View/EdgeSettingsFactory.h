#include "GuitarEdgeSettingsDialog.h"

#include "src/Configuration.h"

class EdgeSettingsFactory
{
public:
    virtual ~EdgeSettingsFactory() = default;
    virtual EdgeSettingsDialog * create(QWidget * parent) const = 0;
};

class GuitarEdgeSettingsFactory : public EdgeSettingsFactory
{
public:
    EdgeSettingsDialog * create(QWidget * parent) const override
    {
        auto interval = Configuration::intervalFor(Music::Instrument::GUITAR);
        return new GuitarEdgeSettingsDialog(interval, parent);
    }
};
