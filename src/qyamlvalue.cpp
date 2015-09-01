#include <QRegularExpression>
#include "qyaml_p.h"
#include "qyamlmapping.h"
#include "qyamlsequence.h"
#include "qyamlvalue.h"

#define SETOK(p, v) if (p) *p = v;

namespace QtYAML
{

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

    static QRegularExpression trueExpression(
                "^(y(?:es)?|on|t(?:rue)?)$",
                QRegularExpression::CaseInsensitiveOption);
    static QRegularExpression falseExpression(
                "^(no?|off|f(?:alse)?)$",
                QRegularExpression::CaseInsensitiveOption);

    QString s = toString();
    if (trueExpression.match(s).hasMatch())
    {
        SETOK(ok, true);
        return true;
    }
    if (falseExpression.match(s).hasMatch())
    {
        SETOK(ok, true);
        return false;
    }
    return false;
}

QString Value::toString() const
{
    if (isScalar())
    {
        auto v = dynamic_cast<ScalarPrivate *>(d_ptr->data.data());
        if (v)
            return v->data;
    }
    return QString();
}

}   // namespace QtYAML
