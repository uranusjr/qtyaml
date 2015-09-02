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

bool Value::toBool(bool *ok) const
{
    SETOK(ok, false);
    if (!isScalar())
        return false;

    auto v = d_ptr->data.dynamicCast<ScalarPrivate>();
    if (v.isNull())
        return false;

    if (!v->canBeBoolean())
        return false;

    QString s = v->data;
    if (!gBooleanValueMap.contains(s))
        return false;

    SETOK(ok, true);
    return gBooleanValueMap.value(s);
}

int Value::toInt(bool *ok) const
{
    /* YAML integers can be of 5 forms:
     *
     * 1. "Normal" numerical values without leading "0" is decimal.
     * 2. Sequence starting with "0x" is hexadecimal.
     * 3. Sequence starting with "0b" is binary.
     * 4. Sequence starting with "0" (not followed by "x" or "b") is octal.
     * 5. Sets of two digit decimal values separated by ":" form a sexagesimal.
     *
     * Any form can have a prefix of "+" or "-" indicating signedness.
     * Underscores ("_") in the number (non-prefix) sequence are ignored.
     */
    SETOK(ok, false);
    if (!isScalar())
        return 0;

    auto v = d_ptr->data.dynamicCast<ScalarPrivate>();
    if (v.isNull())
        return 0;

    if (!v->canBeInteger())
        return 0;

    // Ignore underscores.
    QString s = v->data.replace('_', "");

    // Calculate signedness.
    int sign = 1;
    if (s.startsWith('-'))
    {
        s.remove(0, 1);
        sign = -1;
    }
    else if (s.startsWith('+'))
    {
        s.remove(0, 1);
        sign = 1;
    }

    // Special case 0, +0, and -0.
    if (s == "0")
    {
        SETOK(ok, true);
        return 0;
    }

    // Either binary, hexadecimal, or octal.
    if (s.startsWith('0'))
    {
        s.remove(0, 1);

        int base = 8;
        if (s.startsWith('b'))
        {
            s.remove(0, 1);
            base = 2;
        }
        else if (s.startsWith('x'))
        {
            s.remove(0, 1);
            base = 16;
        }

        bool iok = false;
        int result = s.toInt(&iok, base);
        SETOK(ok, iok);
        return iok ? sign * result : 0;
    }

    // Sexagesimal.
    if (s.contains(':'))
    {
        int unit = 1;
        int result = 0;

        QStringList parts = s.split(':');
        for (auto it = parts.constEnd(); it != parts.constBegin(); it--)
        {
            bool iok = false;
            int value = (it - 1)->toInt(&iok, 10);
            if (!iok)
                return 0;
            result += value * unit;
            unit *= 60;
        }
        SETOK(ok, true);
        return sign * result;
    }

    // Decimal.
    bool iok = false;
    int result = s.toInt(&iok, 10);

    SETOK(ok, iok);
    return iok ? sign * result : 0;
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
