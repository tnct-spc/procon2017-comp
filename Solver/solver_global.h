#ifndef SOLVER_GLOBAL_H
#define SOLVER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(SOLVER_LIBRARY)
#  define SOLVERSHARED_EXPORT Q_DECL_EXPORT
#else
#  define SOLVERSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // SOLVER_GLOBAL_H
