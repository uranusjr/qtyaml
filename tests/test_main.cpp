#include <QCoreApplication>
#include "qtyamltest.h"

#define RUN(klass, argc, argv) \
    do { \
        klass *obj = new klass(); \
        status |= QTest::qExec(obj, argc, argv); \
        delete obj; \
    } while(0)

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Q_UNUSED(app);

    int status = 0;
    RUN(HighLevelTests, argc, argv);
    RUN(QtYAMLDotOrgTests, argc, argv);
    RUN(QtYAMLPracticalTests, argc, argv);
    return status;
}

