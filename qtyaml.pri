defineReplace(mode) {
    CONFIG(release, debug|release):return(release)
    else:return(debug)
}

CONFIG += c++11

DESTDIR = ../bin/$$mode()

BUILD_DIR = ../build/$$mode()
PRECOMPILED_DIR = $$BUILD_DIR
OBJECTS_DIR = $$BUILD_DIR
MOC_DIR = $$BUILD_DIR
RCC_DIR = $$BUILD_DIR
UI_DIR = $$BUILD_DIR

QMAKE_CFLAGS_WARN_ON += -Wno-unused-parameter

INCLUDEPATH += \
    $$PWD/3rdparty/libyaml/include \
    $$PWD/3rdparty/libyaml/src

# Workaround to avoid running ./configure
DEFINES += HAVE_CONFIG_H
INCLUDEPATH += $$PWD/3rdparty/libyaml/win32

# Add your files here

HEADERS += \
    $$PWD/src/qtyaml_global.h \
    $$PWD/src/qyamldocument.h \
    $$PWD/src/qyamlparser.h \
    $$PWD/src/qyamlsequence.h \
    $$PWD/src/qyaml_p.h \
    $$PWD/src/qyamlmapping.h \
    $$PWD/src/qyamlvalue.h

SOURCES += \
    $$PWD/3rdparty/libyaml/src/api.c \
    $$PWD/3rdparty/libyaml/src/dumper.c \
    $$PWD/3rdparty/libyaml/src/emitter.c \
    $$PWD/3rdparty/libyaml/src/loader.c \
    $$PWD/3rdparty/libyaml/src/parser.c \
    $$PWD/3rdparty/libyaml/src/reader.c \
    $$PWD/3rdparty/libyaml/src/scanner.c \
    $$PWD/3rdparty/libyaml/src/writer.c \
    $$PWD/src/qyamldocument.cpp \
    $$PWD/src/qyamlparser.cpp \
    $$PWD/src/qyamlsequence.cpp \
    $$PWD/src/qyaml.cpp \
    $$PWD/src/qyamlmapping.cpp \
    $$PWD/src/qyamlvalue.cpp
