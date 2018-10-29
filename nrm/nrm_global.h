#ifndef NRM_GLOBAL_H
#define NRM_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NRM_LIBRARY)
#  define NRM_EXPORT Q_DECL_EXPORT
#else
#  define NRM_EXPORT Q_DECL_IMPORT
#endif

#endif // NRM_GLOBAL_H
