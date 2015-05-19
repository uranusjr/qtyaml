#ifndef QYAMLPARSER_H
#define QYAMLPARSER_H

extern "C" {
#include <yaml.h>
}
#include "qyamldocument.h"

namespace QtYAML
{

class Parser
{
public:
    static DocumentList parse(yaml_parser_t *parser, ParseError *error);
};

}   // namespace QtYAML

#endif // QYAMLPARSER_H
