#ifndef QTYAML_GLOBAL_H
#define QTYAML_GLOBAL_H

#include <QtCore/QtGlobal>

#if defined(QTYAML_LIBRARY)
#  define QTYAMLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QTYAMLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QTYAML_GLOBAL_H
