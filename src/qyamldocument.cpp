#include <QList>
#include "qyaml_p.h"
#include "qyamlmapping.h"
#include "qyamlparser.h"
#include "qyamlsequence.h"

namespace QtYAML
{

static yaml_parser_t *yaml_parser_create(ParseError *error)
{
    yaml_parser_t *parser = new yaml_parser_t();
    if (!yaml_parser_initialize(parser))
    {
        if (error)
        {
            error->problem = "Failed to initialize parser.";
            error->offset = 0;
            error->error = ParseError::MemoryError;
        }
        delete parser;
        return 0;
    }
    return parser;
}

class DocumentPrivate
{
    Q_DECLARE_PUBLIC(Document)
    Document * const q_ptr;

public:
    DocumentPrivate(Document *q);
    ~DocumentPrivate();

    QSharedPointer<BasePrivate> root;

    enum {
        EMPTY,
        MAPPING,
        SEQUENCE,
    } rootType;
};

DocumentPrivate::DocumentPrivate(Document *q) :
    q_ptr(q), rootType(EMPTY)
{
}

DocumentPrivate::~DocumentPrivate()
{
}

Document::Document() : d_ptr(new DocumentPrivate(this))
{
}

Document::~Document()
{
    d_ptr.clear();
}

Document::Document(const Document &other) : d_ptr(other.d_ptr)
{
}

Document &Document::operator=(const Document &other)
{
    if (d_ptr != other.d_ptr)
    {
        d_ptr.clear();
        d_ptr = other.d_ptr;
    }
    return *this;
}

Document::Document(const Mapping &mapping) : d_ptr(new DocumentPrivate(this))
{
    d_ptr->root = mapping.d_ptr;
    d_ptr->rootType = DocumentPrivate::MAPPING;
}

Document::Document(const Sequence &sequence) : d_ptr(new DocumentPrivate(this))
{
    d_ptr->root = sequence.d_ptr;
    d_ptr->rootType = DocumentPrivate::SEQUENCE;
}

DocumentList Document::fromYaml(const QByteArray &yaml, ParseError *error)
{
    yaml_parser_t *parser = yaml_parser_create(error);
    if (!parser)
        return DocumentList();

    const uchar *bytes = reinterpret_cast<const uchar *>(yaml.data());
    yaml_parser_set_input_string(parser, bytes, yaml.size());
    DocumentList documents = Parser::parse(parser, error);

    yaml_parser_delete(parser);
    delete parser;
    return documents;
}

QByteArray Document::toYaml() const
{
    // TODO: Implement me.
    return QByteArray();
}

bool Document::isEmpty() const
{
    return d_ptr->rootType == DocumentPrivate::EMPTY;
}

bool Document::isMapping() const
{
    return d_ptr->rootType == DocumentPrivate::MAPPING;
}

bool Document::isSequence() const
{
    return d_ptr->rootType == DocumentPrivate::SEQUENCE;
}

Mapping Document::mapping() const
{
    if (isMapping())
    {
        QSharedPointer<MappingPrivate> map =
                d_ptr->root.dynamicCast<MappingPrivate>();
        if (!map.isNull())
            return Mapping(map);
    }
    return Mapping();
}

Sequence Document::sequence() const
{
    if (isSequence())
    {
        QSharedPointer<SequencePrivate> seq =
                d_ptr->root.dynamicCast<SequencePrivate>();
        if (!seq.isNull())
            return Sequence(seq);
    }
    return Sequence();
}

}   // namespace QtYAML
