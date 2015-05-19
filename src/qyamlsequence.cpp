#include "qyaml_p.h"
#include "qyamlsequence.h"

namespace QtYAML
{

Sequence::Sequence() : d_ptr(new SequencePrivate(this))
{
}

Sequence::Sequence(QSharedPointer<SequencePrivate> d) : d_ptr(d)
{
}

Sequence::~Sequence()
{
    d_ptr.clear();
}

Sequence::Sequence(const Sequence &other) : d_ptr(other.d_ptr)
{
}

Sequence &Sequence::operator=(const Sequence &other)
{
    if (d_ptr != other.d_ptr)
    {
        d_ptr.clear();
        d_ptr = other.d_ptr;
    }
    return *this;
}

int Sequence::size() const
{
    return d_ptr->children.size();
}

Value Sequence::at(int i) const
{
    QSharedPointer<BasePrivate> child = d_ptr->children.at(i);
    return Value(child);
}

Sequence::const_iterator Sequence::constBegin() const
{
    return const_iterator(this, 0);
}

Sequence::const_iterator Sequence::constEnd() const
{
    return const_iterator(this, size());
}

}   // namespace QtYAML
