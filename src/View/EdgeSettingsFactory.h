#include "GuitarEdgeSettingsDialog.h"

class EdgeSettingsFactory
{
public:
    virtual ~EdgeSettingsFactory() = default;
    virtual EdgeSettingsDialog * create(const QPixmap&, QWidget * parent) const = 0;
};

class GuitarEdgeSettingsFactory : public EdgeSettingsFactory
{
public:
    EdgeSettingsDialog * create(const QPixmap& icon, QWidget * parent) const override
    {
        return new GuitarEdgeSettingsDialog(icon, parent);
    }
};
