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

class QtYAMLDotOrgTests : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testBoolCanonical();
    void testBoolAnswer();
    void testBoolLogical();
    void testBoolOption();

    void testIntCanonical();
    void testIntDecimal();
    void testIntOctal();
    void testIntHexadecimal();
    void testIntBinary();
    void testIntSexagesimal();

private:
    DocumentList docs;
};

class QtYAMLPracticalTests : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void testDocument();
    void testSite();
    void testPLCs();
    void testSilos();

private:
    DocumentList docs;
};

#endif  // QTYAMLTEST_H

