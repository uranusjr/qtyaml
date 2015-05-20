#include "qyaml_p.h"
#include "qyamlmapping.h"

namespace QtYAML
{

Mapping::Mapping() : d_ptr(new MappingPrivate(this))
{
}

Mapping::Mapping(QSharedPointer<MappingPrivate> d) : d_ptr(d)
{
}

Mapping::~Mapping()
{
}

Mapping::Mapping(const Mapping &other) : d_ptr(other.d_ptr)
{
}

Mapping &Mapping::operator=(const Mapping &other)
{
    if (d_ptr != other.d_ptr)
        d_ptr = other.d_ptr;
    return *this;
}

int Mapping::size() const
{
    return d_ptr->children.size();
}

Value Mapping::value(const QString &key) const
{
    QSharedPointer<BasePrivate> child = d_ptr->children.value(key);
    return Value(child);
}

Value Mapping::operator[](const QString &key) const
{
    return value(key);
}

}   // namespace QtYAML
