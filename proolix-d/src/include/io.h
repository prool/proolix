/*      io.h

        Definitions for low level I/O functions.

        Copyright (c) Serge Pustovoitoff 1994-1997

*/

#ifndef _IO_H
#define _IO_H 1

#ifndef _MODE_T /* prool */
#define _MODE_T
typedef unsigned int mode_t;
#endif

#ifndef _FTIME_
#define _FTIME_
struct  ftime   {
        unsigned      ft_tsec  : 5;   /* Two second interval */
        unsigned      ft_min   : 6;   /* Minutes */
        unsigned      ft_hour  : 5;   /* Hours */
        unsigned      ft_day   : 5;   /* Days */
        unsigned      ft_month : 4;   /* Months */
        unsigned      ft_year  : 7;   /* Year */
};
#endif

#define SEEK_CUR      1
#define SEEK_END      2
#define SEEK_SET      0

int   access    (const char far *path, int amode); /* prool */
int   _chmod    (const char far *path, int func, ... /* int attr */); /* prool */
int   chmod     (const char far *path, int amode); /* prool */
int   chsize    (int handle, long size);
int   _close    (int handle);
int   close     (int handle);
int   _creat    (const char far *path, mode_t attribute); /* prool */
int   creat     (const char far *path, mode_t amode);     /* prool */
int   dup       (int handle);
int   dup2      (int oldhandle, int newhandle);
int   eof       (int handle);
long  filelength(int handle);
int   getftime  (int handle, struct ftime far *ftimep);   /* prool */
int   ioctl     (int handle, unsigned long func, ...);    /* prool */
/* optional 3rd and 4th args are: void FAR * argdx, int argcx */   /* prool */
int   isatty    (int handle);
long  lseek     (int handle, long offset, int fromwhere);
int   _open     (const char far *path, int oflags);       /* prool */
int   open      (const char far *path, int access,...);   /* prool */
long int  _read (int handle, void far *buf, unsigned long len);/*prool*/
long int   read (int handle, void far *buf, unsigned long len);/*prool*/
int   setftime  (int handle, struct ftime far *ftimep);   /* prool */
int   setmode   (int handle, int amode);
long  tell      (int handle);
unsigned  umask (unsigned cmask);
int   unlink    (const char far *path);
int   unlock    (int handle, long offset, long length);
long int  _write(int handle, void far *buf, unsigned long len);/*prool*/
long int   write(int handle, void far *buf, unsigned long len);/*prool*/

/* macros for compatibility with earlier versions & other compilers. */
#define sopen(path,access,shflag,mode)   open (path, (access)|(shflag), mode)

#endif  /* _IO_H */
