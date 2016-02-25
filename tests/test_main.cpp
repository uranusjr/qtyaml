#include <QCoreApplication>
#include "qtyamltest.h"

#define RUN(klass) \
    do { \
        klass obj; \
        status |= QTest::qExec(&obj, argc, argv); \
    } while(0)

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Q_UNUSED(app);

    int status = 0;
    RUN(HighLevelTests);
    RUN(YAMLDotOrgTests);
    RUN(PracticalTests);
    return status;
}
