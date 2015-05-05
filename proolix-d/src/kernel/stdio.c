#include <stdarg.h>

#include <prool.h>

#include <conf.c>
#include "kernel.h"
/****************************************************************************/
#if 0 /* Это сохранено для истории */

char Hex [] = "0123456789ABCDEF";

void ohb (char c)
{
putch(Hex[c>>4]);
putch(Hex[c&0xf]);
}

void ohw (int w)
{
ohb(w>>8);
ohb(w&0xff);
}

void ohd (unsigned long int ll)
{
ohw(ll>>16);
ohw(ll&0xffff);
}
#endif
/****************************************************************************/
void perror(const char far *str)
{
printf("%s: ",str);
puts(sys_errlist[errno]);
return;
/* потом нужно будет заменить printf на fprintf(stderr, ... */
}
/****************************************************************************/
void ohs(const char far *s)
{
do printf("%2X ",*s); while (*s++);
}
/****************************************************************************/
int unlink(const char far *path)
{int h;

/* Проверка существования файла и проверка, что это не каталог */
if ((h=open(path,O_RDONLY))==-1) {errno=ENOENT; return -1;}
else
  {
  if (FCBS[h].Attr.B.Dir) {close(h); errno=EPERM; return -1;}
  if (close(h)==-1) return -1;
  }

/* Собственно удаление */
return delete(path);
}
/****************************************************************************/
int delete (const char far *path)
{int h;
unsigned long Inode;
DIR far *fp; struct dirent far *D; long Pos;

if ((h=open(path,O_WRONLY|O_TRUNC))==-1) return -1;
Inode=FCBS[h].DirInode;
Pos=FCBS[h].DirSeekPos;
if (close(h)==-1) return -1;

if ((fp=openidir(Inode))==NULL) return -1;
if (seekdir(fp,Pos,SEEK_SET)==-1) return -1;
if ((D=readdir(fp))==NULL) return -1;
(*D).filename[0]=0xE5;
if (seekdir(fp,Pos,SEEK_SET)==-1) return -1;
if (writedir(fp,D)==-1) return -1;
if (closedir(fp)==-1) return -1;
return 0;
}
/****************************************************************************/
int rename(const char far *old, const char far *new)
{int h; struct dirent far *D; DIR far *fp;

if((h=open(old,O_WRONLY))==-1) return -1;
if ((fp=openidir(FCBS[h].DirInode))==NULL) return -1;
if (seekdir(fp,FCBS[h].DirSeekPos,SEEK_SET)==-1) return -1;
if ((D=readdir(fp))==NULL) return -1;
PathToDir(new,D);
if (seekdir(fp,FCBS[h].DirSeekPos,SEEK_SET)==-1) return -1;
if (writedir(fp,D)==-1) return -1;
FCBS[h].Name[0]=0;
if (closedir(fp)==-1) return -1;
return 0;
}