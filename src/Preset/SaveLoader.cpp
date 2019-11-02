#include "SaveLoader.h"

#include <QFile>

#include "Serializer.h"
#include "Presets.h"
#include "FactoryMethod.h"

namespace Preset {

namespace {

using namespace Instruments;

using SGuitar = Serializer<GuitarTag>;
using SElectricGuitar = Serializer<ElectricGuitarTag>;
using SPiano = Serializer<PianoTag>;

class SerializeVisitor: public ConstVisitor
{
public:

    void visit(const GuitarPreset& preset) override
    {
        serialize(preset, SGuitar());
    }

    void visit(const ElectricGuitarPreset& preset) override
    {
        serialize(preset, SElectricGuitar());
    }

    void visit(const PianoPreset& preset) override
    {
        serialize(preset, SPiano());
    }

    QByteArray data() const { return m_data; }

private:

    void serialize(const auto& preset, const auto& serializer)
    {
        m_data = serializer.serialize(preset.data());
    }

    QByteArray m_data;
};

class DeserializeVisitor: public Visitor
{
public:

    void visit(GuitarPreset& preset) override
    {
        deserialize(preset, SGuitar());
    }

    void visit(ElectricGuitarPreset& preset) override
    {
        deserialize(preset, SElectricGuitar());
    }

    void visit(PianoPreset& preset) override
    {
        deserialize(preset, SPiano());
    }

    void setData(const QByteArray& data) { m_data = data; }
    bool result() const { return m_result; }

private:

    void deserialize(auto & preset, const auto& deserializer)
    {
        auto data = deserializer.deserialize(m_data, m_result);

        if(m_result)
            preset.setData(data);
    }

    bool m_result = false;
    QByteArray m_data;
};

}

SaveLoader::SaveLoader() = default;
SaveLoader::~SaveLoader() = default;

AbstractPreset * SaveLoader::load(const QString &filePath)
{
    QFile loadFile(filePath);
    if(!loadFile.open(QFile::ReadOnly))
        return nullptr;

    QByteArray data = loadFile.readAll();

    bool ok = false;
    auto type = readSerializationMeta(data, ok).type;

    if(!ok)
        return nullptr;

    auto preset = createPreset(type);

    auto visitor = DeserializeVisitor();
    visitor.setData(data);
    preset->acceptVisitor(visitor);

    if(!visitor.result())
    {
        delete preset;
        return nullptr;
    }

    return preset;
}

bool SaveLoader::save(const QString &filePath, const AbstractPreset * preset)
{
    if(!preset)
        return false;

    QFile saveFile(filePath);

    if(!saveFile.open(QFile::WriteOnly))
        return false;

    auto visitor = SerializeVisitor();
    preset->acceptVisitor(visitor);

    saveFile.write(visitor.data());

    return true;
}

}
