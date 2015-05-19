#ifndef QYAML_P_H
#define QYAML_P_H

#include <QHash>
#include <QSharedPointer>
#include <QString>

namespace QtYAML
{

class Mapping;
class Sequence;
class Value;

class BasePrivate
{
public:
    virtual ~BasePrivate();
    virtual inline bool isMapping() { return false; }
    virtual inline bool isSequence() { return false; }
};

class MappingPrivate : public BasePrivate
{
    Q_DECLARE_PUBLIC(Mapping)
    Mapping *q_ptr;

public:
    MappingPrivate(Mapping *q = 0);
    virtual inline bool isMapping() { return true; }

    QHash< QString, QSharedPointer<BasePrivate> > children;
};

class SequencePrivate : public BasePrivate
{
    Q_DECLARE_PUBLIC(Sequence)
    Sequence *q_ptr;

public:
    SequencePrivate(Sequence *q = 0);
    virtual inline bool isSequence() { return true; }

    QList< QSharedPointer<BasePrivate> > children;
};

class ScalarPrivate : public BasePrivate
{
public:
    QString data;
};

}   // namespace QtYAML

#endif // QYAML_P_H
