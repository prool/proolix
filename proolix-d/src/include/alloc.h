/*      alloc.h

        memory management functions and variables.

        Copyright (c) Serge Pustovoitoff 1993-1997

*/

#ifndef _STDDEF
#define _STDDEF
#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned long size_t;
#endif
#endif

#ifndef NULL
#if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
#define NULL    0
#else
#define NULL    0L
#endif
#endif

void far* calloc  (size_t nitems, size_t size); /* prool */

unsigned long  coreleft   (void); /* prool */

void      free    (void far *block); /* prool */
void far* malloc  (size_t size);
void far* realloc (void far *block, size_t size); /* prool */
