#ifndef QTYAMLTEST_H
#define QTYAMLTEST_H

#include <QtTest>
#include "qyamldocument.h"

using namespace QtYAML;

class HighLevelTests : public QObject
{
    Q_OBJECT

private slots:
    void testParse();
};

class QtYAMLTests : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void testDocument();
    void testQuotedString();
    void testBoolean();
    void testNonBoolean();
    void testBooleanNoFlag();
    void testSilos();

private:
    DocumentList docs;
};

#endif  // QTYAMLTEST_H

