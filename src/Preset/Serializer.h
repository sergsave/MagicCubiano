#pragma once

#include <QJsonDocument>
#include <QJsonArray>
#include <QVariant>

#include "Model.h"
#include "NotationSerializer.h"

namespace Preset {

struct SerializationMeta
{
    Instruments::Type type;
};

namespace Details {

static QString typeKey()
{
    return "type";
}

}

static SerializationMeta readSerializationMeta(const QByteArray& arr)
{
    QJsonDocument doc = QJsonDocument::fromJson(arr);
    if(doc.isNull())
        return {};

    auto val = doc.object()[Details::typeKey()].toInt();
    return { static_cast<Instruments::Type>(val) };
}

template <typename InstrumentTag>
class Serializer
{
public:

    // TODO : Result check
    // TODO : Without copy ?

    QByteArray serialize(const Data<InstrumentTag>& data) const
    {
        QJsonObject obj;
        auto type = Instruments::typeFrom(InstrumentTag());
        obj[Details::typeKey()] = static_cast<int>(type);
        obj[dataKey] = writeData(data);

        QJsonDocument doc(obj);
        return doc.toJson(QJsonDocument::Compact);
    }

    Data<InstrumentTag> deserialize(const QByteArray& arr) const
    {
        QJsonDocument doc = QJsonDocument::fromJson(arr);
        if(doc.isNull())
            return {};

        return readData(doc.object());
    }

private:
    using TUnit = Unit<InstrumentTag>;
    using TUnits = Units<InstrumentTag>;
    using TColors = QMap<CubeEdge::Color, Units<InstrumentTag>>;
    using TData = Data<InstrumentTag>;

    const QString durationKey = "duration";
    const QString notationKey = "notation";
    const QString dataKey = "data";

    QJsonObject writeUnit(const TUnit& unit) const
    {
        QJsonObject obj;

        QJsonObject notObj;
        serializeNotation(unit.notation, notObj);

        obj[notationKey] = notObj;
        obj[durationKey] = unit.duration;
        return obj;
    }

    QJsonArray writeUnits(const TUnits& units) const
    {
        QJsonArray arr;

        for(auto u: units)
            arr.append(writeUnit(u));

        return arr;
    }

    QJsonObject writeColors(const TColors& colors) const
    {
        QVariantMap vMap;

        for (auto key: colors.keys())
            vMap.insert(QString::number(key), writeUnits(colors.value(key)));

        return QJsonObject::fromVariantMap(vMap);
    }

    QJsonObject writeData(const TData& data) const
    {
        QVariantMap vMap;

        for (auto key: data.keys())
            vMap.insert(QString::number(key), writeColors(data.value(key)));

        return QJsonObject::fromVariantMap(vMap);
    }

    TUnit readUnit(const QJsonObject& obj) const
    {
        TUnit unit;

        deserializeNotation(unit.notation, obj);
        unit.duration = obj[durationKey].toInt();

        return unit;
    }

    TUnits readUnits(const QJsonArray& arr) const
    {
        TUnits units;

        for(auto js: arr)
            units.append(readUnit(js.toObject()));

        return units;
    }

    TColors readColors(const QJsonObject& obj) const
    {
        TColors colors;
        auto vMap = obj.toVariantMap();

        for(auto key: vMap.keys())
        {
            auto value = vMap.value(key).toList();
            auto units = readUnits(QJsonArray::fromVariantList(value));
            colors[static_cast<CubeEdge::Color>(key.toInt())] = units;
        }

        return colors;
    }

    TData readData(const QJsonObject& obj) const
    {
        TData data;
        auto vMap = obj.toVariantMap();

        for(auto key: vMap.keys())
        {
            auto value = vMap.value(key).toMap();
            auto colors = readColors(QJsonObject::fromVariantMap(value));
            data[static_cast<CubeEdge::Rotation>(key.toInt())] = colors;
        }

        return data;
    }
};

}

