#include <QStack>
#include "qyaml_p.h"
#include "qyamlmapping.h"
#include "qyamlparser.h"
#include "qyamlsequence.h"

namespace QtYAML
{

QSharedPointer<SequencePrivate> parseSequence(
        yaml_parser_t *parser, yaml_token_type_t endTokenType,
        ParseError *error);

static inline QString toString(yaml_token_t *token)
{
    const char *bytes =
            reinterpret_cast<const char *>(token->data.scalar.value);
    return QString::fromUtf8(bytes, token->data.scalar.length);
}

bool parseOne(yaml_parser_t *parser, yaml_token_t *token, ParseError *error)
{
    if (!yaml_parser_scan(parser, token))
    {
        // Error. Discard all parsing results and set error.
        if (error)
        {
            error->problem = QString::fromUtf8(parser->problem);
            error->offset = parser->problem_offset;
            error->error = static_cast<ParseError::Error>(parser->error);
        }
        return false;
    }
    return true;
}

QSharedPointer<MappingPrivate> parseMapping(
        yaml_parser_t *parser, yaml_token_type_t endTokenType,
        ParseError *error)
{
    yaml_token_t token;
    QString key = "";   // Null string = next value is key.

    auto map = QSharedPointer<MappingPrivate>::create();
    do
    {
        if (!parseOne(parser, &token, error))
            return QSharedPointer<MappingPrivate>();

        switch (token.type)
        {
        case YAML_KEY_TOKEN:    // Next scalar token is a key!
            key = QString();
            break;
        case YAML_BLOCK_MAPPING_START_TOKEN:    // Inner mapping.
        {
            auto inner = parseMapping(parser, YAML_BLOCK_END_TOKEN, error);
            if (inner.isNull())
                return QSharedPointer<MappingPrivate>();
            map->children.insert(key, inner);
            break;
        }
        case YAML_BLOCK_SEQUENCE_START_TOKEN:   // Inner sequence.
        {
            auto inner = parseSequence(parser, YAML_BLOCK_END_TOKEN, error);
            if (inner.isNull())
                return QSharedPointer<MappingPrivate>();
            map->children.insert(key, inner);
            break;
        }
        case YAML_FLOW_MAPPING_START_TOKEN:    // Inner flow mapping.
        {
            auto inner = parseMapping(
                        parser, YAML_FLOW_MAPPING_END_TOKEN, error);
            if (inner.isNull())
                return QSharedPointer<MappingPrivate>();
            map->children.insert(key, inner);
            break;
        }
        case YAML_FLOW_SEQUENCE_START_TOKEN:   // Inner flow sequence.
        {
            auto inner = parseSequence(
                        parser, YAML_FLOW_SEQUENCE_END_TOKEN, error);
            if (inner.isNull())
                return QSharedPointer<MappingPrivate>();
            map->children.insert(key, inner);
            break;
        }
        case YAML_SCALAR_TOKEN:
            if (key.isNull())   // Key token.
            {
                key = toString(&token);
            }
            else                // Scalar value token.
            {
                auto val = QSharedPointer<ScalarPrivate>::create();
                val->style = token.data.scalar.style;
                val->data = toString(&token);
                map->children.insert(key, val);
            }
            break;
        default:
            // TODO: Error on illogical tokens.
            break;
        }
    } while (token.type != endTokenType);

    return map;
}

QSharedPointer<SequencePrivate> parseSequence(
        yaml_parser_t *parser, yaml_token_type_t endTokenType,
        ParseError *error)
{
    yaml_token_t token;

    auto seq = QSharedPointer<SequencePrivate>::create();
    do
    {
        if (!parseOne(parser, &token, error))
            return QSharedPointer<SequencePrivate>();

        switch (token.type)
        {
        case YAML_BLOCK_MAPPING_START_TOKEN:    // Inner mapping.
        {
            auto inner = parseMapping(parser, YAML_BLOCK_END_TOKEN, error);
            if (inner.isNull())
                return QSharedPointer<SequencePrivate>();
            seq->children.append(inner);
            break;
        }
        case YAML_BLOCK_SEQUENCE_START_TOKEN:   // Inner sequence.
        {
            auto inner = parseSequence(parser, YAML_BLOCK_END_TOKEN, error);
            if (inner.isNull())
                return QSharedPointer<SequencePrivate>();
            seq->children.append(inner);
            break;
        }
        case YAML_FLOW_MAPPING_START_TOKEN:    // Inner flow mapping.
        {
            auto inner = parseMapping(
                        parser, YAML_FLOW_MAPPING_END_TOKEN, error);
            if (inner.isNull())
                return QSharedPointer<SequencePrivate>();
            seq->children.append(inner);
            break;
        }
        case YAML_FLOW_SEQUENCE_START_TOKEN:   // Inner flow sequence.
        {
            auto inner = parseSequence(
                        parser, YAML_FLOW_SEQUENCE_END_TOKEN, error);
            if (inner.isNull())
                return QSharedPointer<SequencePrivate>();
            seq->children.append(inner);
            break;
        }
        case YAML_SCALAR_TOKEN:
        {
            auto val = QSharedPointer<ScalarPrivate>::create();
            val->data = toString(&token);
            seq->children.append(val);
            break;
        }
        default:
            // TODO: Error on illogical tokens.
            break;
        }
    } while (token.type != endTokenType);

    return seq;
}

DocumentList Parser::parse(yaml_parser_t *parser, ParseError *error)
{
    DocumentList documents;

    yaml_token_t token;
    do
    {
        if (!parseOne(parser, &token, error))
            return DocumentList();

        switch (token.type)
        {
        case YAML_NO_EVENT:
            break;

        // Stream start/end.
        case YAML_STREAM_START_TOKEN:
            break;
        case YAML_STREAM_END_TOKEN:
            break;

        // Directives. Ignored for now.
        case YAML_VERSION_DIRECTIVE_TOKEN:
            break;
        case YAML_TAG_DIRECTIVE_TOKEN:
            break;

        // Document delimeters.
        case YAML_DOCUMENT_START_TOKEN:
            break;
        case YAML_DOCUMENT_END_TOKEN:
            break;

        // Blocks entries. Delegate to parse functions.
        case YAML_BLOCK_SEQUENCE_START_TOKEN:
        {
            auto seq = parseSequence(parser, YAML_BLOCK_END_TOKEN, error);
            if (seq.isNull())
                return DocumentList();
            documents.append(Document(Sequence((seq))));
            break;
        }
        case YAML_BLOCK_MAPPING_START_TOKEN:
        {
            auto map = parseMapping(parser, YAML_BLOCK_END_TOKEN, error);
            if (map.isNull())
                return DocumentList();
            documents.append(Document(Mapping(map)));
            break;
        }

        // Flow entries. Delegate to parse functions.
        case YAML_FLOW_SEQUENCE_START_TOKEN:
        {
            auto seq = parseSequence(
                        parser, YAML_FLOW_SEQUENCE_END_TOKEN, error);
            if (seq.isNull())
                return DocumentList();
            documents.append(Document(Sequence((seq))));
            break;
        }
        case YAML_FLOW_MAPPING_START_TOKEN:
        {
            auto map = parseMapping(
                        parser, YAML_FLOW_MAPPING_END_TOKEN, error);
            if (map.isNull())
                return DocumentList();
            documents.append(Document(Mapping(map)));
            break;
        }

        default:
            // TODO: Error on illogical tokens.
            break;
        }
    } while (token.type != YAML_STREAM_END_TOKEN);

    yaml_token_delete(&token);
    return documents;
}

}   // namespace QtYAML
