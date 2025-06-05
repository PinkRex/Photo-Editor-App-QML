#ifndef ERODE_GLOBAL_H
#define ERODE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ERODE_LIBRARY)
#define ERODE_EXPORT Q_DECL_EXPORT
#else
#define ERODE_EXPORT Q_DECL_IMPORT
#endif

#endif // ERODE_GLOBAL_H
