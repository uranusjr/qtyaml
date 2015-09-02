#include <QRegularExpression>
#include "qyaml_p.h"
#include "qyamlmapping.h"
#include "qyamlsequence.h"
#include "qyamlvalue.h"

#define SETOK(p, v) if (p) *p = v;

namespace QtYAML
{

static QHash<QString, bool> getBooleanValueMap()
{
    QHash<QString, bool> map;
    map["y"] = true;
    map["Y"] = true;
    map["yes"] = true;
    map["Yes"] = true;
    map["YES"] = true;
    map["n"] = false;
    map["N"] = false;
    map["no"] = false;
    map["No"] = false;
    map["NO"] = false;
    map["true"] = true;
    map["True"] = true;
    map["TRUE"] = true;
    map["false"] = false;
    map["False"] = false;
    map["FALSE"] = false;
    map["on"] = true;
    map["On"] = true;
    map["ON"] = true;
    map["off"] = false;
    map["Off"] = false;
    map["OFF"] = false;
    return map;
}

static QHash<QString, bool> gBooleanValueMap = getBooleanValueMap();


ValuePrivate::ValuePrivate(Value *q) : q_ptr(q), data(0)
{
}

bool Value::isUndefined() const
{
    return d_ptr->data.isNull();
}

bool Value::isMapping() const
{
    return !d_ptr->data.isNull() && d_ptr->data->isMapping();
}

bool Value::isSequence() const
{
    return !d_ptr->data.isNull() && d_ptr->data->isSequence();
}

bool Value::isScalar() const
{
    return d_ptr->data.isNull() || !d_ptr->data->isMapping()
            || !d_ptr->data->isSequence();
}

Value::Value(QSharedPointer<BasePrivate> data) : d_ptr(new ValuePrivate(this))
{
    d_ptr->data = data;
}

Value::Value() : d_ptr(new ValuePrivate(this))
{
}

Value::~Value()
{
}

Mapping Value::toMapping() const
{
    if (isMapping())
    {
        auto map = d_ptr->data.dynamicCast<MappingPrivate>();
        if (!map.isNull())
            return Mapping(map);
    }
    return Mapping();
}

Sequence Value::toSequence() const
{
    if (isSequence())
    {
        auto seq = d_ptr->data.dynamicCast<SequencePrivate>();
        if (!seq.isNull())
            return Sequence(seq);
    }
    return Sequence();
}

bool Value::toBoolean(bool *ok) const
{
    SETOK(ok, false);
    if (!isScalar())
        return false;

    auto data = d_ptr->data.dynamicCast<ScalarPrivate>();
    if (data.isNull())
        return false;

    if (!data->canBeBoolean())
        return false;

    QString s = toString();
    if (!gBooleanValueMap.contains(s))
        return false;

    SETOK(ok, true);
    return gBooleanValueMap.value(s);
}

QString Value::toString(bool *ok) const
{
    SETOK(ok, false);
    if (!isScalar())
        return QString();

    auto v = d_ptr->data.dynamicCast<ScalarPrivate>();
    if (v.isNull())
        return QString();

    SETOK(ok, true);
    return v->data;
}

}   // namespace QtYAML
