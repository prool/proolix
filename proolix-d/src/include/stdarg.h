/*      stdarg.h

        Definitions for accessing parameters in functions that accept
        a variable number of arguments.

        Copyright (c) Serge Pustovoitoff 1993-1997

*/
#if     !defined(__STDARG)
#define __STDARG

typedef void far *va_list; /* prool */

#define va_start(ap, parmN)     (ap = ...)
#if 0
#define va_arg(ap, type)        (*((type *)(ap))++)
#else
#define va_arg(ap, type)        (*((type far *)(ap))++)
#endif
#define va_end(ap)
#define _va_ptr                 (...)
#endif
