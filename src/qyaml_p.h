#ifndef QYAML_P_H
#define QYAML_P_H

#include <QHash>
#include <QSharedPointer>
#include <QString>
extern "C" {
#include <yaml.h>
}

namespace QtYAML
{

class Mapping;
class Sequence;
class Value;

class BasePrivate
{
public:
    virtual ~BasePrivate();
    virtual inline bool isMapping() const { return false; }
    virtual inline bool isSequence() const { return false; }
};

class MappingPrivate : public BasePrivate
{
    Q_DECLARE_PUBLIC(Mapping)
    Mapping *q_ptr;

public:
    MappingPrivate(Mapping *q = 0);
    virtual inline bool isMapping() const { return true; }

    QHash< QString, QSharedPointer<BasePrivate> > children;
};

class SequencePrivate : public BasePrivate
{
    Q_DECLARE_PUBLIC(Sequence)
    Sequence *q_ptr;

public:
    SequencePrivate(Sequence *q = 0);
    virtual inline bool isSequence() const { return true; }

    QList< QSharedPointer<BasePrivate> > children;
};

class ScalarPrivate : public BasePrivate
{
public:
    ScalarPrivate();
    inline bool canBeBoolean() const { return mayBePlain(); }
    inline bool canBeInteger() const;

    inline bool mayBePlain() const;

    yaml_scalar_style_t style;
    QString data;
};

bool ScalarPrivate::canBeInteger() const
{
    if (!mayBePlain())
        return false;
    if (data.isEmpty())
        return false;
    return true;
}

bool ScalarPrivate::mayBePlain() const
{
    switch (style)
    {
    case YAML_ANY_SCALAR_STYLE:
    case YAML_PLAIN_SCALAR_STYLE:
        return true;
    default:
        break;
    }
    return false;
}

class ValuePrivate
{
    Q_DECLARE_PUBLIC(Value)
    Value *q_ptr;

public:
    ValuePrivate(Value *q);

    QSharedPointer<BasePrivate> data;
};

}   // namespace QtYAML

#endif // QYAML_P_H
