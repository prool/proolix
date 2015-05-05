/*      dirent.h

        by Serge Pustovoitoff 1995-2006
*/

#include <stdio.h>

#ifndef _DIRENT_H_
#define _DIRENT_H_

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

struct  fdate   {
        unsigned        ft_day   : 5;   /* Days */
        unsigned        ft_month : 4;   /* Months */
        unsigned        ft_year  : 7;   /* Year */
};

#define d_ino           d_fileno        /* backward compatibility */
#define DIR FILE
struct dirent /* logical directory structure */
{
char filename [11];
union FileAttr attrib;
struct ftime FileUpdateDateTime;
unsigned long FirstClu; /* initial cluster number */
unsigned long Size;
};

struct dirent16 /* directory structure of FAT-12 and FAT-16 */
{
char d_name [11];
union FileAttr Attr;
char Reserv [10];
struct ftime FileDateTime;
unsigned int d_fileno; /* initial cluster number */
unsigned long Size;
};

struct dirent32 /* directory structure of FAT-32 */
{
char d_name [11];
union FileAttr Attr;
unsigned char NT;
unsigned char Fignya; /* десятые доли времени создания */
struct ftime CreationDateTime;
struct fdate LastReadDate;
unsigned int FirstCluHi; /* initial cluster number */
struct ftime UpdateDateTime;
unsigned int FirstCluLo; /* initial cluster number */
unsigned long Size;
};

DIR far *opendir (const char far *dirname);
#ifdef KERNEL
DIR far *openwdir (const char far *dirname);
#endif
DIR far *openidir (long inode);
struct dirent far *readdir (const DIR far *dirp);
#ifdef KERNEL
int writedir (const DIR far *dirp, struct dirent far *D);
#endif
void rewinddir (const DIR far *dirp);
int closedir (const DIR far *dirp);
long telldir (const DIR far *dirp);
long seekdir (const DIR far *dirp, long offset, int where);
int dircmp (struct dirent d1, struct dirent d2);

#endif /* !_DIRENT_H_ */