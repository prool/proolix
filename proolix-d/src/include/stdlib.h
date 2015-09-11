/*      stdlib.h

        Definitions for common types, variables, and functions.

        Copyright (c) Serge Pustovoitoff 1994-1997

*/

#if     !defined(__STDLIB)
#define __STDLIB

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned long size_t;
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

/* Maximum value returned by "rand" function
*/
#define RAND_MAX 0x7FFF

typedef void  (* atexit_t)(void);

int        abs    (int x);
double     atof   (const char *s);
int        htoi   (const char far *s); /* prool */
long int   htol   (const char far *s); /* prool */
int        atoi   (const char far *s); /* prool */
long       atol   (const char far *s); /* prool */
void far * calloc (size_t nitems, size_t size); /* prool */
div_t      div    (int numer, int denom);
void       exit   (int status);
void       free   (void far *block); /* prool */
char far * getenv (const char far *name); /* prool */
long       labs   (long x);
ldiv_t     ldiv   (long numer, long denom);
void far * malloc (size_t size); /* prool */
void       qsort  (void far *base, size_t nelem, size_t width,/*prool*/
                         int /**/ (far *fcmp)
                         (/* const void far *, const void far * */));
int        rand   (void);
void far * realloc(void far *block, size_t size); /* prool */
void       srand  (unsigned seed);
double     strtod (const char *s, char **endptr);
long       strtol (const char *s, char **endptr, int radix);
int        system (const char far *command);
unsigned long  strtoul (const char *s, char **endptr, int radix);

#if !__STDC__

#ifndef NULL
#if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
#define NULL    0
#else
#define NULL    0L
#endif
#endif

/* Variables */

extern  int              errno;
extern  char          ** environ;
extern  char           * sys_errlist[]; /* prool */

#define abs(x)          (((x) < (0)) ? (-(x)) : (x)) /* prool */
#define labs(x)         (((x) < (0)) ? (-(x)) : (x)) /* prool */

#define max(a,b)        (((a) > (b)) ?   (a)  : (b))
#define min(a,b)        (((a) < (b)) ?   (a)  : (b))

#define random(num)     (rand() % (num))
#define randomize()     srand((unsigned)time(NULL))

char     * ecvt  (double value, int ndig, int *dec, int *sign);
void       _exit (int status);
char     * fcvt  (double value, int ndig, int *dec, int *sign);
char     * gcvt  (double value, int ndec, char *buf);
char far * itoa  (int value, char far *string, int radix); /* prool */
void     * lfind (const void *key, const void *base,
                        size_t *num, size_t width,
                        int  (*fcmp)(/* const void *, const void * */));

unsigned long  _lrotl(unsigned long val, int count);
unsigned long  _lrotr(unsigned long val, int count);

char far * ltoa   (long value, char far *string, int radix); /* prool */
int        putenv (const char *name);

unsigned  _rotl   (unsigned value, int count);
unsigned  _rotr   (unsigned value, int count);

char far * ultoa  (unsigned long value, char far *string, int radix); /* prool */
#endif

#endif
