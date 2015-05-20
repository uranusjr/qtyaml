#ifndef QYAMLSEQUENCE_H
#define QYAMLSEQUENCE_H

#include <QSharedPointer>
#include "qyamlvalue.h"

namespace QtYAML
{

class SequencePrivate;

class Sequence
{
    Q_DECLARE_PRIVATE(Sequence)
    QSharedPointer<SequencePrivate> d_ptr;

    Sequence(QSharedPointer<SequencePrivate> d);

public:
    Sequence();
    ~Sequence();

    Sequence(const Sequence &other);
    Sequence &operator=(const Sequence &other);

    // Properties.
    int size() const;
    inline int count() const { return size(); }
    inline int length() const { return size(); }

    // Random access.
    Value at(int i) const;
    inline Value operator[](int i) const { return at(i); }

    // Const iteration.
    class const_iterator;
    const_iterator constBegin() const;
    const_iterator constEnd() const;
    inline const_iterator begin() const { return constBegin(); }
    inline const_iterator end() const { return constEnd(); }

    class const_iterator
    {
        const Sequence * const a;
        int i;

    public:
        inline const_iterator(const Sequence *a, int i) : a(a), i(i) {}
        inline const_iterator(const const_iterator &o) : a(o.a), i(o.i) {}

        inline Value operator*() const { return a->at(i); }
        inline bool operator==(const const_iterator &o) const;
        inline bool operator!=(const const_iterator &o) const;
        inline const_iterator &operator++();
        inline const_iterator operator++(int);
    };

private:
    friend class Document;
    friend class Parser;
    friend class Value;
    friend class const_iterator;
};

bool QtYAML::Sequence::const_iterator::operator==(
        const Sequence::const_iterator &o) const
{
    return i == o.i;
}

bool Sequence::const_iterator::operator!=(
        const Sequence::const_iterator &o) const
{
    return i != o.i;
}

Sequence::const_iterator &Sequence::const_iterator::operator++()
{
    ++i;
    return *this;
}

Sequence::const_iterator Sequence::const_iterator::operator++(int)
{
    const_iterator n = *this;
    ++i;
    return n;
}

}   // namespace QtYAML

#endif // QYAMLSEQUENCE_H
