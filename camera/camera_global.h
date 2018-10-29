#ifndef CAMERA_GLOBAL_H
#define CAMERA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CAMERA_LIBRARY)
#  define CAMERA_EXPORT Q_DECL_EXPORT
#else
#  define CAMERA_EXPORT Q_DECL_IMPORT
#endif

#endif // CAMERA_GLOBAL_H
