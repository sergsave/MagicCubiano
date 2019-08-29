#include "GuitarEdgeSettingsDialog.h"

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
        return new GuitarEdgeSettingsDialog(parent);
    }
};
