#ifndef CORE_GLOBAL
#define CORE_GLOBAL

#include <qglobal.h>

#ifdef CORE_LIB
# define CORE_EXPORT Q_DECL_EXPORT
#else
# define CORE_EXPORT Q_DECL_IMPORT
#endif

#endif // CORE_GLOBAL

