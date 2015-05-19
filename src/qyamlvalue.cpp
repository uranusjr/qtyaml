#include "qyaml_p.h"
#include "qyamlmapping.h"
#include "qyamlsequence.h"
#include "qyamlvalue.h"

namespace QtYAML
{

class ValuePrivate
{
    Q_DECLARE_PUBLIC(Value)
    Value *q_ptr;

public:
    ValuePrivate(Value *q);
    void setData(QSharedPointer<BasePrivate> d);

private:
    QSharedPointer<BasePrivate> data;
    enum {
        SCALAR,
        MAPPING,
        SEQUENCE,
    } type;
};

ValuePrivate::ValuePrivate(Value *q) : q_ptr(q), data(0), type(SCALAR)
{
}

void ValuePrivate::setData(QSharedPointer<BasePrivate> d)
{
    data = d;
    if (d.isNull())
        type = SCALAR;
    else if (d->isMapping())
        type = MAPPING;
    else if (d->isSequence())
        type = SEQUENCE;
    else
        type = SCALAR;
}

Value::Value(QSharedPointer<BasePrivate> data) : d_ptr(new ValuePrivate(this))
{
    d_ptr->setData(data);
}

Value::Value() : d_ptr(new ValuePrivate(this))
{
}

Value::~Value()
{
    d_ptr.clear();
}

Mapping Value::toMapping() const
{
    if (d_ptr->type == ValuePrivate::MAPPING)
    {
        auto map = d_ptr->data.dynamicCast<MappingPrivate>();
        if (!map.isNull())
            return Mapping(map);
    }
    return Mapping();
}

Sequence Value::toSequence() const
{
    if (d_ptr->type == ValuePrivate::SEQUENCE)
    {
        auto seq = d_ptr->data.dynamicCast<SequencePrivate>();
        if (!seq.isNull())
            return Sequence(seq);
    }
    return Sequence();
}

QString Value::toString() const
{
    if (d_ptr->type == ValuePrivate::SCALAR)
    {
        auto v = dynamic_cast<ScalarPrivate *>(d_ptr->data.data());
        if (v)
            return v->data;
    }
    return QString();
}

}   // namespace QtYAML
