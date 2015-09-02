#include <QDebug>
#include "qtyamltest.h"
#include "qyamlmapping.h"
#include "qyamlsequence.h"

void HighLevelTests::testParse()
{
    QFile f(":/public.yaml");
    f.open(QIODevice::ReadOnly);
    QByteArray yaml = f.readAll();
    f.close();

    DocumentList docs = Document::fromYaml(yaml);
    QCOMPARE(docs.size(), 1);

    Document doc = docs.first();
    QVERIFY(doc.isMapping());

    Mapping mapping = doc.mapping();
    QCOMPARE(mapping.size(), 7);

    QCOMPARE(mapping["title"].toString(), QString("Finex 2011"));
    QCOMPARE(mapping["img_url"].toString(), QString("/finex/html/img/"));
    QCOMPARE(mapping["css_url"].toString(), QString("/finex/html/style/"));
    QCOMPARE(mapping["js_url"].toString(), QString("/finex/html/js/"));
    QCOMPARE(mapping["template_dir"].toString(), QString("html/templ/"));
    QCOMPARE(mapping["default_act"].toString(), QString("idx"));

    Sequence pages = mapping["pages"].toSequence();
    QCOMPARE(pages.size(), 4);

    QStringList actionValues =
            QStringList() << "idx" << "reg" << "log" << "out";
    QStringList titleValues =
            QStringList() << "Welcome" << "Register" << "Log in" << "Log out";
    QStringList htmlValues =
            QStringList() << "welcome" << "register" << "login" << "logout";
    for (int i = 0; i < 4; i++)
    {
        Mapping page = pages[i].toMapping();
        QCOMPARE(page["act"].toString(), actionValues[i]);
        QCOMPARE(page["title"].toString(), titleValues[i]);
        QCOMPARE(page["html"].toString(),
                QString("public/%1.phtml").arg(htmlValues[i]));
    }
}


void QtYAMLTests::initTestCase()
{
    QFile f(":/structure.yml");
    f.open(QIODevice::ReadOnly);
    QByteArray yaml = f.readAll();
    f.close();

    docs = Document::fromYaml(yaml);
}

void QtYAMLTests::testDocument()
{
    QCOMPARE(docs.size(), 1);
    Document doc = docs.first();
    QVERIFY(doc.isMapping());
}

void QtYAMLTests::testQuotedString()
{
    Sequence plcs = docs.first().mapping()["plcs"].toSequence();
    QString host = plcs.first().toMapping()["host"].toString();
    QCOMPARE(host, QString("localhost"));
}

void QtYAMLTests::testBoolean()
{
    Mapping site = docs.first().mapping()["site"].toMapping();

    bool ok = false;
    QVERIFY(site["ambient"].toBoolean(&ok));
    QVERIFY(ok);

    ok = false;
    QCOMPARE(site["whatever"].toBoolean(&ok), false);
    QVERIFY(ok);
}

void QtYAMLTests::testNonBoolean()
{
    Mapping site = docs.first().mapping()["site"].toMapping();

    Value nameValue = site["name"];
    QCOMPARE(nameValue.toString(), QString("true"));

    bool ok = false;
    QCOMPARE(nameValue.toBoolean(&ok), false);
    QCOMPARE(ok, false);
}

void QtYAMLTests::testBooleanNoFlag()
{
    Mapping site = docs.first().mapping()["site"].toMapping();

    QCOMPARE(site["ambient"].toBoolean(), true);
    QCOMPARE(site["whatever"].toBoolean(), false);
    QCOMPARE(site["name"].toBoolean(), false);
}

void QtYAMLTests::testSilos()
{
    int siloIndex = 0;
    int siloIds[3] = {1, 2, 3};
    int cableLengths[3] = {2, 1, 3};
    QList<int> cableIds[3] = {
        QList<int>() << 1 << 2,
        QList<int>() << 1,
        QList<int>() << 1 << 2 << 3,
    };
    QList<int> nodeCounts[3] = {
        QList<int>() << 2 << 1,
        QList<int>() << 4,
        QList<int>() << 1 << 2 << 3,
    };

    Sequence silos = docs.first().mapping()["silos"].toSequence();
    QCOMPARE(silos.size(), 3);
    foreach (const Value &v, silos)
    {
        Mapping silo = v.toMapping();

        bool ok = false;
        QCOMPARE(silo["id"].toString().toInt(&ok), siloIds[siloIndex]);
        QVERIFY(ok);

        // TODO: Implement this.
//        QCOMPARE(silo["location"].toString(), locations[siloIndex]);

        int cableIndex = 0;
        Sequence cables = silo["cables"].toSequence();
        foreach (const Value &v, cables)
        {
            Mapping cable = v.toMapping();

            bool ok = false;
            QCOMPARE(cable["id"].toString().toInt(&ok),
                    cableIds[siloIndex][cableIndex]);
            QVERIFY(ok);

            QCOMPARE(cable["node-count"].toString().toInt(&ok),
                    nodeCounts[siloIndex][cableIndex]);
            QVERIFY(ok);

            cableIndex++;
        }

        QCOMPARE(cableIndex, cableLengths[siloIndex]);
        siloIndex++;
    }
    QCOMPARE(siloIndex, 3);
}
