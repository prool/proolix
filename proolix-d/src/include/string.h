/*      string.h

        Definitions for memory and string functions.

        Copyright (c) Serge Pustovoitoff 1994-1997

*/

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned long size_t;
#endif

void far * memccpy (void far *dest,const void far *src,int c, size_t n);
void far * memchr  (const void far *s, int c, size_t n);
int        memcmp  (const void far *s1, const void far *s2, size_t n);
void far * memcpy  (void far *dest, const void far *src, size_t n);
int        memicmp (const void far *s1, const void far *s2, size_t n);
void far * memmove (void far *dest, const void far *src, size_t n);
void far * memset  (void far *s, int c, size_t n);
void       movedata(unsigned srcseg, unsigned srcoff, unsigned dstseg,
                          unsigned dstoff, size_t n);
char far * stpcpy  (char far *dest, const char far *src);
char far * strcat  (char far *dest, const char far *src);
char far * strchr  (const char far *s, int c);
int        strcmp  (const char far*s1, const char far*s2);
char far * strcpy  (char far*dest, const char far*src);
size_t     strcspn (const char far *s1, const char far *s2);
char far * strdup  (const char far *s);
char far * strerror(int errnum);
int        stricmp (const char far *s1, const char far *s2);
size_t     strlen  (const char far *s);
char far * strlwr  (char far *s);
char far * strncat (char far *dest, const char far *src, size_t maxlen);
int        strncmp (const char far *s1, const char far *s2,
                          size_t maxlen);
char far * strncpy (char far*dest, const char far*src, size_t maxlen);
int        strnicmp(const char far *s1, const char far *s2,
                          size_t maxlen);
char far * strnset (char far *s, int ch, size_t n);
char far * strpbrk (const char far *s1, const char far *s2);
char far * strrchr (const char far *s, int c);
char far * strrev  (char far *s);
char far * strset  (char far *s, int ch);
size_t     strspn  (const char far *s1, const char far *s2);
char far * strstr  (const char far *s1, const char far *s2);
char far * strtok  (char far *s1, const char far *s2);
char far * strupr  (char far *s);

char    far * _strerror (const char far *s);

/* compatibility with other compilers */

#define strcmpi(s1,s2)          stricmp(s1,s2)
#define strncmpi(s1,s2,n)       strnicmp(s1,s2,n)
