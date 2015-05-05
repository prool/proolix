/*      dir.h

        Defines structures, macros, and functions for dealing with
        directories and pathnames.

        Copyright (c) Serge Pustovoitoff 1995-1997

*/
#include <sys/types.h>

#if     !defined(__DIR_DEF_)
#define __DIR_DEF_

struct  ffblk   {
        char            ff_reserved[21];
        char            ff_attrib;
        unsigned        ff_ftime;
        unsigned        ff_fdate;
        long            ff_fsize;
        char            ff_name[13];
};

#define WILDCARDS 0x01
#define EXTENSION 0x02
#define FILENAME  0x04
#define DIRECTORY 0x08
#define DRIVE     0x10

int       chdir           (const char far *path); /* prool */
int       getcurdir       (int drive, char *directory);
char far* getcwd          (char far *buf, int buflen);
int       mkdir           (const char far *path, mode_t mode);/*prool*/
int       rmdir           (const char far *path); /* prool */

#endif
