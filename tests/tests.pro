QT       += testlib

QT       -= gui

TARGET    = qtyamltest
CONFIG   += console testcase
CONFIG   -= app_bundle

TEMPLATE  = app

include(../qtyaml.pri)

DEFINES += SRCDIR=\\\"$$PWD/../src\\\"

INCLUDEPATH += $$PWD/../src

SOURCES += \
    qtyamltest.cpp \
    test_main.cpp

HEADERS += \
    qtyamltest.h

RESOURCES += \
    testassets.qrc
