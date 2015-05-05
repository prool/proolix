#include <prool.h>

#include <sys/types.h>
#include <sys/stat.h>

#include <conf.c>
#include "kernel.h"

#ifdef notyet
#error Preprocessor variable 'notyet' is not supported in Proolix
#endif

int fstat (int h, struct stat far *statbuf)
{
if (statbuf==NULL) {errno=EFAULT; return -1;}
if ((h<0)||(h>OPEN_MAX)) {errno=EBADF; return -1;}
if (!FCBS[h].Name[0]) {errno=EBADF; return -1;}

(*statbuf).st_dev   = 0;               /* inode's device */
(*statbuf).st_ino   = FCBS[h].BegClu;  /* inode's number */
(*statbuf).st_mode  = FCBS[h].Attr.U;  /* inode protection mode */
(*statbuf).st_nlink = 1;               /* number of hard links */
(*statbuf).st_uid   = 0;               /* user ID of the file's owner */
(*statbuf).st_gid   = 0;               /* group ID of the file's group */
(*statbuf).st_atime = 0;               /* time of last access */
(*statbuf).st_mtime = 0;               /* time of last data modification */
(*statbuf).st_ctime = 0;               /* time of last file status change */
(*statbuf).st_size  = FCBS[h].Size;    /* file size, in bytes */

return 0;
}

int stat  (char far *path, struct stat far *statbuf)
{int i, h;
if ((h=open(path,O_RDONLY))==-1) {errno=ENOENT; return -1;}
i=fstat(h,statbuf);
close(h);
return i;
}
