#include <QDebug>
#include "qyaml_p.h"

namespace QtYAML
{

BasePrivate::~BasePrivate()
{
}

MappingPrivate::MappingPrivate(Mapping *q) : q_ptr(q)
{
}

SequencePrivate::SequencePrivate(Sequence *q) : q_ptr(q)
{
}

}   // namespace QtYAML
