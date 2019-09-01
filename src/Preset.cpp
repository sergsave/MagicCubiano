#include "Preset.h"
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>

namespace {

const QString g_noteKey = "note";
const QString g_octaveKey = "octave";
const QString g_delayKey = "delay";
const QString g_tonesKey = "tones";

}

using namespace Music;
using Harmonies = QMap<CubeEdge::Color, Music::Harmony>;

QJsonObject writeTone(const Music::Tone& tone)
{
    QJsonObject obj;
    obj[g_noteKey] = static_cast<int>(tone.note);
    obj[g_octaveKey] = tone.octave;
    return obj;
}

QJsonObject writeHarmony(const Music::Harmony& har)
{
    QJsonObject obj;
    QJsonArray tonesArray;

    for(auto tone: har.tones)
        tonesArray.append(writeTone(tone));

    obj[g_tonesKey] = tonesArray;
    obj[g_delayKey] = har.delayMSec;

    return obj;
}

QJsonObject writeHarmonies(const Harmonies& colors)
{
    QVariantMap vMap;
    QMapIterator<CubeEdge::Color, Music::Harmony> it(colors);

    while (it.hasNext())
    {
        it.next();
        vMap.insert(QString::number(it.key()), writeHarmony(it.value()));
    }

    return QJsonObject::fromVariantMap(vMap);
}

QJsonObject writePreset(const Preset &preset)
{
    QVariantMap vMap;
    QMapIterator<CubeEdge::Rotation, Harmonies> it(preset);

    while (it.hasNext())
    {
        it.next();
        vMap.insert(QString::number(it.key()), writeHarmonies(it.value()));
    }

    return QJsonObject::fromVariantMap(vMap);
}

QByteArray presetToJson(const Preset &preset)
{
    QJsonDocument doc(writePreset(preset));
    return doc.toJson(QJsonDocument::Compact);
}

Tone readTone(const QJsonObject& obj)
{
    Tone tone;

    tone.note = static_cast<Tone::Note>(obj[g_noteKey].toInt());
    tone.octave = obj[g_octaveKey].toInt();

    return tone;
}

Harmony readHarmony(const QJsonObject& obj)
{
    Harmony harmony;

    harmony.delayMSec = obj[g_delayKey].toInt();
    auto tonesArr = obj[g_tonesKey].toArray();

    for(auto t: tonesArr)
        harmony.tones.append(readTone(t.toObject()));

    return harmony;
}

Harmonies readHarmonies(const QJsonObject& obj)
{
    Harmonies harmonies;
    auto vMap = obj.toVariantMap();

    for(auto key: vMap.keys())
    {
        auto value = vMap.value(key).toMap();
        auto harmony = readHarmony(QJsonObject::fromVariantMap(value));
        harmonies[static_cast<CubeEdge::Color>(key.toInt())] = harmony;
    }

    return harmonies;
}

Preset readPreset(const QJsonObject& obj)
{
    Preset preset;
    auto vMap = obj.toVariantMap();

    for(auto key: vMap.keys())
    {
        auto value = vMap.value(key).toMap();
        auto harmonies = readHarmonies(QJsonObject::fromVariantMap(value));
        preset[static_cast<CubeEdge::Rotation>(key.toInt())] = harmonies;
    }

    return preset;
}

Preset presetFromJson(const QByteArray & arr, bool &ok)
{
    ok = false;
    QJsonDocument doc = QJsonDocument::fromJson(arr);
    if(doc.isNull())
        return {};

    ok = true;
    return readPreset(doc.object());
}
