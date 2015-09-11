/*      stdio.h

        Definitions for stream input/output.

        Copyright (c) Serge Pustovoitoff 1994-1997

*/

#if     !defined(__STDIO_DEF_)
#define __STDIO_DEF_

#include <limits.h>

#ifndef _FTIME_
#define _FTIME_
struct  ftime   {
        unsigned        ft_tsec  : 5;   /* Two second interval */
        unsigned        ft_min   : 6;   /* Minutes */
        unsigned        ft_hour  : 5;   /* Hours */
        unsigned        ft_day   : 5;   /* Days */
        unsigned        ft_month : 4;   /* Months */
        unsigned        ft_year  : 7;   /* Year */
};
#endif

#ifndef _ATTRFIELD_
#define _ATTRFIELD_
struct AttrField
{
unsigned int ReadOnly : 1;
unsigned int Hidden   : 1;
unsigned int System   : 1;
unsigned int Label    : 1;
unsigned int Dir      : 1;
unsigned int Archive  : 1;
unsigned int Attr2    : 1;
unsigned int Attr1    : 1;
};

union FileAttr
{
struct AttrField B;
unsigned char U;
};
#endif

#ifndef _SIZE_T
#define _SIZE_T
typedef unsigned long size_t;
#endif
#ifndef NULL
#   if defined(__TINY__) || defined(__SMALL__) || defined(__MEDIUM__)
#   define      NULL    0
#   else
#   define      NULL    0L
#   endif
#endif

#if     !defined(__STDARG)
#include        <stdarg.h>
#endif /* prool */

/* Definition of the control structure for streams
*/
typedef struct
{char Name [NAME_MAX+1];
char File;
/* file type:
 0 - device driver,
 1 - ordinary file,
 2 - directory,
 3 - MS DOS root directory */
/* int Major;
   int Minor; */
union FileAttr Attr;
int flag;
struct ftime FileDateTime;
unsigned long BegClu; /* для обычных файлов и каталогов - начальный кластер,
для специальных файлов - это номер спец. файла в таблице */
unsigned long int Size;
unsigned int Drive; /* or filesystem */
/* номер inode (нач.кластера) каталога с записью об этом файле */
unsigned long DirInode;
/* номер позиции записи об этом файле в каталоге (см. opendir() и seekdir() */
long DirSeekPos;
unsigned long AbsClu;
unsigned long AbsSec;
unsigned long AbsOffset;
char Buf [32];
} FILE;

/* End-of-file constant definition
*/
#define EOF     (-1)                    /* End of file indicator */

/* Constants to be used as 3rd argument for "fseek" function
*/
#define SEEK_CUR        1
#define SEEK_END        2
#define SEEK_SET        0

#if 1 /* prool */
extern FILE far *stdin ;
extern FILE far *stdout;
extern FILE far *stderr;
#else
/* Standard I/O predefined streams for MSDOS TurboC 2.0 */
extern  FILE     _streams[];

#define stdin   (&_streams[0])
#define stdout  (&_streams[1])
#define stderr  (&_streams[2])
#define stdaux  (&_streams[3])
#define stdprn  (&_streams[4])
#endif

void      ohd (unsigned long int ll); /* prool */
void      ohs (const char far *s); /* prool */

typedef long fpos_t;

int       fgetpos  (FILE far *stream, fpos_t far *pos); /* prool */
int       fsetpos  (FILE far *stream, const fpos_t far *pos); /* prool */

int       fclose   (FILE far *stream); /* prool */
int       fflush   (FILE far *stream); /* prool */
int       fgetc    (FILE far *stream); /* prool */
FILE far* fopen    (const char far *path, const char far *mode); /* prool */
int       fprintf  (FILE far *stream, const char far *format, ...); /* prool */
int       fputc    (int c, FILE far *stream); /* prool */
int       fputs    (const char far *s, FILE far *stream); /* prool */
size_t    fread    (void far *ptr, size_t size, size_t n, FILE far *stream); /* prool */
FILE far* freopen  (const char far *path, const char far *mode,
                          FILE far *stream); /* prool */
int       fscanf   (FILE *stream, const char *format, ...);
int       fseek    (FILE far *stream, long offset, int wh); /* prool */
long      ftell    (FILE far *stream); /* prool */
size_t    fwrite   (const void far *ptr, size_t size, size_t n,
                          FILE far *stream); /* prool */
#ifdef KERNEL /* proolix */
char     *   gets    (char     *s);
char     *  fgets    (char     *s, int n, FILE far *stream);
#else
char far *   gets    (char far *s);
char far *  fgets    (char far *s, int n, FILE far *stream);
#endif

char far* _gets    (char far *s); /* prool */
void      perror   (const char far *s); /* prool */
int       printf   (const char far *format, ...); /* prool */
int       puts     (const char far *s); /* prool */
#define puts0(s) printf(s) /* Это для совместимости сПрулем (спрулем - это наречие) */
int       Puts0    (const char far *str); /* Prool */
int       puti     (int w, int Prec, int Zeroes); /* prool */
int       putl     (long int w, int Prec, int Zeroes); /* prool */
int       rename   (const char far *oldname, const char far *newname); /* prool */
void      rewind   (FILE far *stream); /* prool */
int       scanf    (const char *format, ...);
int       sprintf  (char far *buffer, const char far *format, ...); /* prool */
int       sscanf   (const char *buffer, const char *format, ...);
char far* strerror (int errnum);
int       ungetc   (int c, FILE *stream);

int       vfprintf (FILE *stream, const char *format, va_list arglist); /* prool */
int       vfscanf  (FILE *stream, const char *format, va_list arglist);
int       vprintf  (const char far *format, va_list arglist); /* prool */
int       vscanf   (const char *format, va_list arglist);
int       vsprintf (char far *buffer, const char far *format, va_list arglist); /* prool */
int       vsscanf  (const char *buffer, const char *format, va_list arglist);

#if !__STDC__
int       fcloseall(void);
FILE far* fdopen   (int handle, char far *type); /* prool */
int       fgetchar (void);
int       flushall (void);
int       fputchar (int c);
int       getw     (FILE *stream);
int       putw     (int w, FILE *stream);
char far* _strerror(const char far *s);
int       unlink   (const char far *path);   /* prool */
#ifdef KERNEL
int       delete   (const char far *path);   /* prool */
#endif

#endif

int       _fgetc   (FILE *stream);           /* used by getc() macro */
int       _fputc   (char c, FILE *stream);   /* used by putc() macro */

int       fileno   (const FILE far *stream); /* prool */

/*      The following macros provide for common functions */

#ifdef Proolix
int feof   (FILE far *stream);
int ferror (FILE far *stream);
#else
#define ferror(f)       ((f)->flags & _F_ERR)
#define feof(f)         ((f)->flags & _F_EOF)
#endif

#define remove(path)    unlink(path)

#define getc(f) fgetc(f)
#define putc(c,f) fputc(c,f)

int putchar (int);
int getchar (void);

#define ungetc(c,f)     ungetc((c),f)   /* traditionally a macro */

extern  int       More;           /* prool */
extern  int       NLine;          /* prool */

#endif