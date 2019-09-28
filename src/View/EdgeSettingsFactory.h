#include "GuitarEdgeSettingsDialog.h"

class EdgeSettingsFactory
{
public:
    virtual ~EdgeSettingsFactory() = default;
    virtual EdgeSettingsDialog * create(QWidget * parent) const = 0;

    static EdgeSettingsFactory * createInstance(Music::Instrument ins);
};
