#include <QCoreApplication>
#include "qtyamltest.h"

#define RUN(klass, argc, argv) \
    { \
        klass *obj = new klass(); \
        status |= QTest::qExec(obj, argc, argv); \
        delete obj; \
    }

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Q_UNUSED(app);

    int status = 0;
    RUN(HighLevelTests, argc, argv);
    RUN(QtYAMLTests, argc, argv)
    return status;
}

