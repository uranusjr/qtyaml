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

    bool isUndefined() const;
    bool isMapping() const;
    bool isSequence() const;
    bool isScalar() const;

    Mapping toMapping() const;
    Sequence toSequence() const;

    bool toBoolean(bool *ok = 0) const;
    QString toString() const;

private:
    friend class Mapping;
    friend class Sequence;
};

}   // namespace QtYAML

#endif // QYAMLVALUE_H
