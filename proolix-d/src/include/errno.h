/*      errno.h

        Defines the system error variable errno and the error
        numbers set by system calls.

        Copyright (c) Serge Pustovoitoff 1993-1997

*/

/*  Dos Error Codes */

#define EZERO    0              /* Error 0 - no error           */
#define EINVFNC  1              /* Invalid function number      */
#define ENOFILE  2              /* File not found               */
#define ENOPATH  3              /* Path not found               */
#define ECONTR   7              /* Memory blocks destroyed      */
#define EINVMEM  9              /* Invalid memory block address */
#define EINVENV 10              /* Invalid environment          */
#define EINVFMT 11              /* Invalid format               */
#define EINVACC 12              /* Invalid access code          */
#define EINVDAT 13              /* Invalid data                 */
#define EINVDRV 15              /* Invalid drive specified      */
#define ECURDIR 16              /* Attempt to remove CurDir     */
#define ENOTSAM 17              /* Not same device              */
#define ENMFILE 18              /* No more files                */

#define ENOENT   2              /* No such file or directory    */
#define EMFILE   4              /* Too many open files          */
#define EACCES   5              /* Permission denied            */
#define EBADF    6              /* Bad file number              */
#define ENOMEM   8              /* Not enough core              */
#define ENODEV  15              /* No such device               */
#define EINVAL  19              /* Invalid argument             */
#define E2BIG   20              /* Arg list too long            */
#define ENOEXEC 21              /* Exec format error            */
#define EXDEV   22              /* Cross-device link            */
#define EDOM    33              /* Math argument                */
#define ERANGE  34              /* Result too large             */
#define EEXIST  35              /* File already exists          */

#define EFAULT  36              /* Unknown error, I/O error, Proolix error */
#define EPERM   37              /* UNIX and Proolix error       */
#define ESRCH   38              /* UNIX and Proolix error       */
#define EINTR   39              /* UNIX and Proolix error       */
#define EIO     40              /* UNIX and Proolix error       */
#define ENXIO   41              /* UNIX and Proolix error       */
#define ECHILD  42              /* UNIX and Proolix error       */
#define EAGAIN  43              /* UNIX and Proolix error       */
#define ENOTBLK 44              /* UNIX and Proolix error       */
#define EBUSY   45              /* UNIX and Proolix error       */
#define ENOTDIR 46              /* UNIX and Proolix error       */
#define EISDIR  47              /* UNIX and Proolix error       */
#define ENFILE  48              /* UNIX and Proolix error       */
#define ENOTTY  49              /* UNIX and Proolix error       */
#define ETXTBSY 50              /* UNIX and Proolix error       */
#define EFBIG   51              /* UNIX and Proolix error       */
#define ENOSPC  52              /* UNIX and Proolix error       */
#define ESPIPE  53              /* UNIX and Proolix error       */
#define EROFS   54              /* UNIX and Proolix error       */
#define EMLINK  55              /* UNIX and Proolix error       */
#define EPIPE   56              /* UNIX and Proolix error       */
#define EUCLEAN 57              /* UNIX and Proolix error       */

#define _sys_nerr 57            /* highest defined system error number */

extern  int        errno;
