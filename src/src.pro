QT      += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    CONFIG += c++11
}

TARGET   = qtyaml
TEMPLATE = lib

include(../qtyaml.pri)

DEFINES += QTYAML_LIBRARY

unix {
    target.path = /usr/lib
    INSTALLS += target
}

HEADERS +=
