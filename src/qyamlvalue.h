#ifndef QYAMLVALUE_H
#define QYAMLVALUE_H

#include <QSharedPointer>

namespace QtYAML
{

class Mapping;
class Sequence;
class BasePrivate;

class ValuePrivate;

class Value
{
    Q_DECLARE_PRIVATE(Value)
    QSharedPointer<ValuePrivate> d_ptr;

    Value(QSharedPointer<BasePrivate> data);

public:
    Value();
    ~Value();

    friend class Mapping;
    friend class Sequence;

    Mapping toMapping() const;
    Sequence toSequence() const;
    QString toString() const;
};

}   // namespace QtYAML

#endif // QYAMLVALUE_H
