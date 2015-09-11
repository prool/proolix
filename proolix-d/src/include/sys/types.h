/*      types.h

        Types.

        Copyright (c) Serge Pustovoitoff 1994-1997

*/

#ifndef  __TIME_T
#define  __TIME_T
typedef long    time_t;
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned long size_t;
#endif

#ifndef _MODE_T /* prool */
#define _MODE_T
typedef unsigned int mode_t;
#endif

#ifndef  __CLOCK_T
#define  __CLOCK_T
typedef long clock_t;
#endif

#ifndef _DIV_T
#define _DIV_T
typedef struct {
        int     quot;
        int     rem;
} div_t;
#endif

#ifndef _LDIV_T
#define _LDIV_T
typedef struct {
        long    quot;
        long    rem;
} ldiv_t;
#endif

#ifndef _TYPES_H /* prool */
#define _TYPES_H

#ifndef _CC_T /* prool */
#define _CC_T
typedef unsigned char cc_t;
#endif

typedef int dev_t;
typedef int gid_t;
typedef unsigned long ino_t;
typedef unsigned nlink_t;
typedef unsigned long off_t;
typedef int pid_t;
typedef void far *sigjmp_buf;
typedef unsigned speed_t;
typedef unsigned tcflag_t;
typedef int uid_t;
typedef unsigned wchar_t;
#endif