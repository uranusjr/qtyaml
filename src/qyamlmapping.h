#ifndef QYAMLMAPPING_H
#define QYAMLMAPPING_H

#include <QSharedPointer>
#include "qyamlvalue.h"

namespace QtYAML
{

class MappingPrivate;

class Mapping
{
    Q_DECLARE_PRIVATE(Mapping)
    QSharedPointer<MappingPrivate> d_ptr;

    Mapping(QSharedPointer<MappingPrivate> d);

public:
    Mapping();
    ~Mapping();

    Mapping(const Mapping &other);
    Mapping &operator=(const Mapping &other);

    int size() const;
    inline int count() const { return size(); }
    inline int length() const { return size(); }

    Value value(const QString &key) const;
    Value operator[](const QString &key) const;

private:
    friend class Document;
    friend class Parser;
    friend class Value;
};

}   // namespace QtYAML

#endif // QYAMLMAPPING_H
