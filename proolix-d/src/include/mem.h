/*      mem.h

        Memory manipulation functions

        Copyright (c) Serge Pustovoitoff 1994-1997

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

void far *  memccpy (void far *dest, const void far *src, int c,
                           size_t n);
void far *  memchr  (const void far *s, int c, size_t n);
int         memcmp  (const void far *s1, const void far *s2, size_t n);
void far *  memcpy  (void far *dest, const void far *src, size_t n);
int         memicmp (const void far *s1, const void far *s2, size_t n);
void far *  memmove (void far *dest, const void far *src, size_t n);
void far *  memset  (void far *s, int c, size_t n);
void        movedata(unsigned srcseg, unsigned srcoff, unsigned dstseg,
                           unsigned dstoff, size_t n);
void        movmem  (void far *src, void far *dest, unsigned length);
void        setmem  (void far *dest, unsigned length, char value);
