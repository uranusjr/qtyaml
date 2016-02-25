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


void YAMLDotOrgTests::initTestCase()
{
    QFile f(":/yaml.org.yml");
    f.open(QIODevice::ReadOnly);
    QByteArray yaml = f.readAll();
    f.close();

    docs = Document::fromYaml(yaml);
}

void YAMLDotOrgTests::testBoolCanonical()
{
    Mapping values = docs.first().mapping()["bool"].toMapping();
    bool ok = false;
    QCOMPARE(values["canonical"].toBool(&ok), true);
    QVERIFY(ok);
}

void YAMLDotOrgTests::testBoolAnswer()
{
    Mapping values = docs.first().mapping()["bool"].toMapping();
    bool ok = false;
    QCOMPARE(values["answer"].toBool(&ok), false);
    QVERIFY(ok);
}

void YAMLDotOrgTests::testBoolLogical()
{
    Mapping values = docs.first().mapping()["bool"].toMapping();
    bool ok = false;
    QCOMPARE(values["logical"].toBool(&ok), true);
    QVERIFY(ok);
}

void YAMLDotOrgTests::testBoolOption()
{
    Mapping values = docs.first().mapping()["bool"].toMapping();
    bool ok = false;
    QCOMPARE(values["option"].toBool(&ok), true);
    QVERIFY(ok);
}

void YAMLDotOrgTests::testIntCanonical()
{
    Mapping values = docs.first().mapping()["int"].toMapping();
    bool ok = false;
    QCOMPARE(values["canonical"].toInt(&ok), 685230);
    QVERIFY(ok);
}

void YAMLDotOrgTests::testIntDecimal()
{
    Mapping values = docs.first().mapping()["int"].toMapping();
    bool ok = false;
    QCOMPARE(values["decimal"].toInt(&ok), 685230);
    QVERIFY(ok);
}

void YAMLDotOrgTests::testIntOctal()
{
    Mapping values = docs.first().mapping()["int"].toMapping();
    bool ok = false;
    QCOMPARE(values["octal"].toInt(&ok), 685230);
    QVERIFY(ok);
}

void YAMLDotOrgTests::testIntHexadecimal()
{
    Mapping values = docs.first().mapping()["int"].toMapping();
    bool ok = false;
    QCOMPARE(values["hexadecimal"].toInt(&ok), 0x0a74ae);
    QVERIFY(ok);
}

void YAMLDotOrgTests::testIntBinary()
{
    Mapping values = docs.first().mapping()["int"].toMapping();
    bool ok = false;
    QCOMPARE(values["binary"].toInt(&ok), 685230);
    QVERIFY(ok);
}

void YAMLDotOrgTests::testIntSexagesimal()
{
    Mapping values = docs.first().mapping()["int"].toMapping();
    bool ok = false;
    QCOMPARE(values["sexagesimal"].toInt(&ok), 685230);
    QVERIFY(ok);
}

void PracticalTests::initTestCase()
{
    QFile f(":/structure.yml");
    f.open(QIODevice::ReadOnly);
    QByteArray yaml = f.readAll();
    f.close();

    docs = Document::fromYaml(yaml);
}

void PracticalTests::testDocument()
{
    QCOMPARE(docs.size(), 1);
    Document doc = docs.first();
    QVERIFY(doc.isMapping());
}

void PracticalTests::testSite()
{
    Mapping site = docs.first().mapping()["site"].toMapping();

    bool ok;

    ok = false;
    QVERIFY(site["ambient"].toBool(&ok));
    QVERIFY(ok);

    Value nameValue = site["name"];
    QCOMPARE(nameValue.toString(), QString("true"));

    ok = false;
    QCOMPARE(nameValue.toBool(&ok), false);
    QCOMPARE(ok, false);
}

void PracticalTests::testPLCs()
{
    QList<QVariantHash> plcInfos;

    QVariantHash plcInfo;
    plcInfo["id"] = 1;
    plcInfo["host"] = "localhost";
    plcInfo["port"] = 9999;
    plcInfo["slave"] = "01";
    plcInfos << plcInfo;

    plcInfo.clear();
    plcInfo["id"] = 2;
    plcInfo["host"] = "localhost";
    plcInfo["port"] = 9998;
    plcInfo["slave"] = "01";
    plcInfos << plcInfo;

    bool ok;
    int i = 0;
    foreach (const Value &value, docs.first().mapping()["plcs"].toSequence())
    {
        Mapping plc = value.toMapping();
        QVariantHash info = plcInfos[i];

        QCOMPARE(plc["id"].toString(), info["id"].toString());
        QCOMPARE(plc["host"].toString(), info["host"].toString());
        QCOMPARE(plc["port"].toString(), info["port"].toString());
        QCOMPARE(plc["slave"].toString(), info["slave"].toString());

        ok = false;
        QCOMPARE(plc["id"].toInt(&ok), info["id"].toInt());
        QVERIFY(ok);

        ok = false;
        QCOMPARE(plc["port"].toInt(&ok), info["port"].toInt());
        QVERIFY(ok);

        ok = true;
        plc["slave"].toInt(&ok);
        QVERIFY(!ok);

        i++;
    }
}

void PracticalTests::testSilos()
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
        QCOMPARE(silo["id"].toInt(&ok), siloIds[siloIndex]);
        QVERIFY(ok);

        // TODO: Implement this.
//        QCOMPARE(silo["location"].toString(), locations[siloIndex]);

        int cableIndex = 0;
        Sequence cables = silo["cables"].toSequence();
        foreach (const Value &v, cables)
        {
            Mapping cable = v.toMapping();

            bool ok = false;
            QCOMPARE(cable["id"].toInt(&ok),
                     cableIds[siloIndex][cableIndex]);
            QVERIFY(ok);

            QCOMPARE(cable["node-count"].toInt(&ok),
                     nodeCounts[siloIndex][cableIndex]);
            QVERIFY(ok);

            cableIndex++;
        }

        QCOMPARE(cableIndex, cableLengths[siloIndex]);
        siloIndex++;
    }
    QCOMPARE(siloIndex, 3);
}

void PracticalTests::testAirConditioners()
{
    Sequence acs = docs.first().mapping()["air-conditioners"].toSequence();
    QCOMPARE(acs.size(), 1);

    Mapping ac = acs.first().toMapping();

    bool ok = false;
    QCOMPARE(ac["id"].toInt(&ok), 1);
    QVERIFY(ok);

    Sequence silos = ac["silos"].toSequence();
    QCOMPARE(silos.size(), 3);

    int ids[3] = {1, 2, 3};
    Sequence::const_iterator begin = silos.constBegin();
    for (Sequence::const_iterator it = begin; it != silos.constEnd(); it++)
    {
        bool ok = false;
        QCOMPARE((*it).toInt(&ok), ids[it - begin]);
        QVERIFY(ok);
    }
}
