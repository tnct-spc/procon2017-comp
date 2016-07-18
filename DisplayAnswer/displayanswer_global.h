#ifndef DISPLAYANSWER_GLOBAL_H
#define DISPLAYANSWER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DISPLAYANSWER_LIBRARY)
#  define DISPLAYANSWERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DISPLAYANSWERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DISPLAYANSWER_GLOBAL_H