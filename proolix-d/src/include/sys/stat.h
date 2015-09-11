/*      stat.h

        Definitions used for file status functions

        Copyright (c) Serge Pustovoitoff 1995-1997

*/

#include <sys/types.h>

#ifndef _STAT_H
#define _STAT_H 1

#define S_IFMT  0xF000  /* file type mask */
#define S_IFDIR 0x4000  /* directory */
#define S_IFIFO 0x1000  /* FIFO special */
#define S_IFCHR 0x2000  /* character special */
#define S_IFBLK 0x3000  /* block special */
#define S_IFREG 0x8000  /* or just 0x0000, regular */
#define S_IREAD 0x0100  /* owner may read */
#define S_IWRITE 0x0080 /* owner may write */
#define S_IEXEC 0x0040  /* owner may execute <directory search> */

int   fstat (int handle, struct stat far *statbuf); /* prool */
int   stat  (char far *path, struct stat far *statbuf); /* prool */
#define lstat stat

#if 1
      /* Modified by Prool */

#define S_ISDIR(m)      (m & FA_DIREC)                  /* directory */

                        /* regular file */
#define S_ISREG(m)      (!((m & FA_DIREC)||(m & FA_LABEL)))

                        /* fifo: include both fifo and socket (pipe) */
   #if 1
   #define S_ISFIFO(m)     (0)
   #define S_ISLNK(m)      (0)                             /* symbolic link */
   #define S_ISCHR(m)      (0)                             /* char special */
   #define S_ISBLK(m)      (0)                             /* block special */
   #define S_ISSOCK(m)     (0)                             /* socket */
   #endif
#define S_ISLABEL(m)    (m & FA_LABEL)                  /* device label */

/*
 * Current stat structure with larger uid/gid/dev fields;
 * still 32-bit offsets and sizes, so we reserve space to increase later.
 */
struct stat {
        dev_t   st_dev;                 /* inode's device */
        ino_t   st_ino;                 /* inode's number */
        mode_t  st_mode;                /* inode protection mode */
        nlink_t st_nlink;               /* number of hard links */
        uid_t   st_uid;                 /* user ID of the file's owner */
        gid_t   st_gid;                 /* group ID of the file's group */
        dev_t   st_rdev;                /* device type */
#ifdef notyet
        struct  timespec st_atimespec;  /* time of last access */
        struct  timespec st_mtimespec;  /* time of last data modification */
        struct  timespec st_ctimespec;  /* time of last file status change */
#else
        time_t  st_atime;               /* time of last access */
        long    st_spare1;
        time_t  st_mtime;               /* time of last data modification */
        long    st_spare2;
        time_t  st_ctime;               /* time of last file status change */
        long    st_spare3;
#endif
        off_t   st_size;                /* file size, in bytes */
        long    st_size1;               /* reserved for st_size expansion */
        long    st_blocks;              /* blocks allocated for file */
        long    st_blocks1;             /* reserved for st_blocks expansion */
        unsigned long st_blksize;       /* optimal blocksize for I/O */
        unsigned long st_flags;         /* user defined flags for file */
        unsigned long st_gen;           /* file generation number */
        long    st_lspare;
        long    st_qspare[2*2];
};

#endif

int  mkdir (const char far *path, mode_t mode); /* prool */

#endif  /* _STAT_H */
