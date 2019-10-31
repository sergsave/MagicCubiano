#pragma once

#include <QJsonDocument>
#include <QJsonArray>
#include <QVariant>

#include "Presets.h"
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

static SerializationMeta readSerializationMeta(const QByteArray& arr, bool& ok)
{
    ok = false;

    QJsonDocument doc = QJsonDocument::fromJson(arr);
    if(doc.isNull())
        return {};

    auto obj = doc.object();
    auto key = Details::typeKey();

    if(!obj.contains(key))
        return {};

    ok = true;
    auto val = obj[key].toInt();
    return { static_cast<Instruments::Type>(val) };
}

template <typename InstrumentTag>
class Serializer
{
public:
    // TODO : Without copy ?

    QByteArray serialize(const typename TPreset<InstrumentTag>::Data& data) const
    {
        QJsonObject obj;
        auto type = Instruments::typeFrom(InstrumentTag());
        obj[Details::typeKey()] = static_cast<int>(type);
        obj[dataKey] = writeData(data);

        QJsonDocument doc(obj);
        return doc.toJson(QJsonDocument::Compact);
    }

    typename TPreset<InstrumentTag>::Data deserialize(const QByteArray& arr, bool& ok) const
    {
        ok = false;
        QJsonDocument doc = QJsonDocument::fromJson(arr);
        if(doc.isNull())
            return {};

        ok = true;
        auto obj = doc.object();
        auto dataObj = obj[dataKey].toObject();

        return readData(dataObj);
    }

private:
    using TUnit = typename TPreset<InstrumentTag>::Unit;
    using TColors = QMap<CubeEdge::Color, TUnit>;
    using TData = typename TPreset<InstrumentTag>::Data;

    const QString durationKey = "duration";
    const QString notationsKey = "notations";
    const QString dataKey = "data";

    QJsonObject writeUnit(const TUnit& unit) const
    {
        QJsonObject obj;

        QJsonArray notationsArr;
        for(auto n: unit.notations)
        {
            QJsonObject obj;
            serializeNotation(n, obj);
            notationsArr.append(obj);
        }

        obj[notationsKey] = notationsArr;
        obj[durationKey] = unit.minDurationMSec;
        return obj;
    }

    QJsonObject writeColors(const TColors& colors) const
    {
        QVariantMap vMap;

        for (auto key: colors.keys())
            vMap.insert(QString::number(key), writeUnit(colors.value(key)));

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

        unit.minDurationMSec = obj[durationKey].toInt();

        auto notationsJsArray = obj[notationsKey].toArray();
        for(auto value: notationsJsArray)
        {
            typename InstrumentTag::Notation notation;
            deserializeNotation(notation, value.toObject());
            unit.notations.append(notation);
        }

        return unit;
    }

    TColors readColors(const QJsonObject& obj) const
    {
        TColors colors;
        auto vMap = obj.toVariantMap();

        for(auto key: vMap.keys())
        {
            auto value = vMap.value(key).toMap();
            auto units = readUnit(QJsonObject::fromVariantMap(value));
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

