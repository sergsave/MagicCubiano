#include "SaveLoader.h"

#include "Serializer.h"
#include "Storage.h"
#include "Model.h"

namespace Preset {

namespace {

using namespace Instruments;

using SGuitar = Serializer<GuitarTag>;
using SElectricGuitar = Serializer<ElectricGuitarTag>;
using SPiano = Serializer<PianoTag>;

class SerializeVisitor: public Visitor
{
public:

    void visit(GuitarPreset * preset) override
    {
        serialize(preset, SGuitar());
    }

    void visit(ElectricGuitarPreset * preset) override
    {
        serialize(preset, SElectricGuitar());
    }

    void visit(PianoPreset * preset) override
    {
        serialize(preset, SPiano());
    }

    QByteArray data() const { return m_data; }

private:

    void serialize(auto * preset, const auto& serializer)
    {
        m_data = serializer.serialize(preset->data());
    }

    QByteArray m_data;
};

class DeserializeVisitor: public Visitor
{
public:

    void visit(GuitarPreset * preset) override
    {
        deserialize(preset, SGuitar());
    }

    void visit(ElectricGuitarPreset * preset) override
    {
        deserialize(preset, SElectricGuitar());
    }

    void visit(PianoPreset * preset) override
    {
        deserialize(preset, SPiano());
    }

    void setData(const QByteArray& data) { m_data = data; }

private:

    void deserialize(auto * preset, const auto& deserializer)
    {
        preset->setData(deserializer.deserialize(m_data));
    }

    QByteArray m_data;
};

}

SaveLoader::SaveLoader(Storage * storage) : m_storage(storage)
{}

SaveLoader::~SaveLoader() = default;

bool SaveLoader::load(const QString& dir, const QString& name)
{
    // TODO: open file
    QByteArray data;

    auto type = readSerializationMeta(data).type;

    auto visitor = DeserializeVisitor();
    visitor.setData(data);

    auto preset = createPreset(type);
    preset->acceptVisitor(&visitor);

//    m_storage->addPreset(name, preset);

    return true;
}

bool SaveLoader::save(const QString& dir, const QString& name)
{
    auto visitor = SerializeVisitor();
    auto preset = m_storage->findPreset(name);

    preset->acceptVisitor(&visitor);

    QByteArray data;
    data = visitor.data();

    // TODO: data to file

    return true;
}

}
