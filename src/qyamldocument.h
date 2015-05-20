#ifndef QYAMLDOCUMENT_H
#define QYAMLDOCUMENT_H

#include <QSharedPointer>
#include <QString>
#include "qtyaml_global.h"

namespace QtYAML
{

class Document;
class Mapping;
class Sequence;

struct QTYAMLSHARED_EXPORT ParseError
{
    // Maps to LibYAML errors.
    enum Error
    {
        NoError,
        MemoryError,
        ReaderError,
        ScannerError,
        ParserError,
        ComposerError,
        WriterError,
        EmitterError,
    };

    QString errorString() const;

    size_t offset;
    QString problem;
    Error error;
};


typedef QList<Document> DocumentList;


class DocumentPrivate;

class QTYAMLSHARED_EXPORT Document
{
    Q_DECLARE_PRIVATE(Document)
    QSharedPointer<DocumentPrivate> d_ptr;

public:
    Document();
    explicit Document(const Mapping &mapping);
    explicit Document(const Sequence &sequence);
    ~Document();

    Document(const Document &other);
    Document &operator=(const Document &other);

    static DocumentList fromYaml(const QByteArray &yaml, ParseError *error = 0);
//    QByteArray toYaml() const;

    bool isEmpty() const;
    bool isMapping() const;
    bool isSequence() const;

    Mapping mapping() const;
    Sequence sequence() const;

    void setMapping(const Mapping &mapping);
    void setSequence(const Sequence &sequence);
};

}   // namespace QtYAML

#endif // QYAMLDOCUMENT_H
