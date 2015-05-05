#include <prool.h>

#include <conf.c>

#include "kernel.h"
/****************************************************************************/
/* #define DEBUG */
/****************************************************************************/
char far *GetUpDir(unsigned long dir_inode, char far *tail)
{
char b1 [PATH_MAX+1];
char b2 [NAME_MAX+1];

if (dir_inode==0)
  {
  strcpy(b1,"/");
  strcat(b1,tail);
  return b1;
  }
else
  {struct dirent far *D; DIR far *dir;
  if (chdir("..")==-1) return NULL;
  if ((dir=opendir("."))==NULL) return NULL;
  for(;;)
    {
    if ((D=readdir(dir))==NULL) break;
    if ((*D).FirstClu==dir_inode)
      {
      strcpy(b1,DirToPath(D,b2));
      if (strlen(tail)!=0)
        {
        strcat(b1,"/");
        strcat(b1,tail);
        }
      return (closedir(dir)==EOF)?NULL:GetUpDir(CurDirClu,b1);
      }
    }
  closedir(dir);
  return NULL;
  }
}
/****************************************************************************/
char Buf2 [PATH_MAX+1];
/****************************************************************************/
char far *getcwd (char far *buf, int buflen)
{
char far *cc; unsigned long SaveDir;

if (buf==NULL) goto L_INVAL;
else if (buflen<=0)
  {L_INVAL: printf("getcwd:EINVAL\n"); errno=EINVAL; return NULL;}
else if (buflen<(PATH_MAX+1))
  {printf("getcwd:ERANGE\n"); errno=ERANGE; return NULL;}

#ifdef DEBUG
printf("getcwd: CurDirClu=%i\n", CurDirClu);
#endif

if (CurDirClu!=0)
  {
  SaveDir=CurDirClu;
  if ((cc=GetUpDir(CurDirClu,""))==NULL) return NULL;
  strcpy(Buf2,cc);

  #ifdef DEBUG
  printf("getcwd: Buf2=%s\n",Buf2);
  printf("getcwd: cc=%Fs\n",cc);
  #endif

  CurDirClu=SaveDir;
  }
else
  {
  strcpy(Buf2,"/");
  }

#ifdef DEBUG
printf("pwd=%s\n",Buf2);
#endif

strncpy(buf,Buf2,buflen);

#ifdef DEBUG
printf("pwd=%Fs\n",buf);
#endif

return buf;
}
/****************************************************************************/
int chdir (const char far *path)
{int i, h;
unsigned long SaveDir;
char far *cc;
char LocalDir0 [PATH_MAX+1];
char LocalDir [PATH_MAX+1];

#ifdef DEBUG
printf("chdir to `%Fs'\n",path);
#endif

if (path==NULL) {errno=ENOENT; return -1;}
if (*path==0) return 0;

strncpy(LocalDir0, path, PATH_MAX+1);

#ifdef DEBUG
printf("LocalDir0=");
printf(LocalDir0);
printf("\n");
#endif

/* Transform: "/usr/bin/" -> "/usr/bin"
              "/"         -> "/"
              "/usr/bin"  -> "/usr/bin" */
i=(int)strlen(LocalDir0)-1;
if (i!=0) if (LocalDir0[i]=='/') LocalDir0[i]=0;

#ifdef DEBUG
printf("LocalDir0=`%s'\n",LocalDir0);
#endif

SaveDir=CurDirClu;

if ((cc=strchr(LocalDir0,'/'))!=NULL)
  { /* full pathname (with '/' separator[s] ) */
  if (cc==LocalDir0)
    {
    CurDirClu=0; /* cd 1 */
    }
  else
    {
    for (i=0; i<=PATH_MAX; i++) LocalDir[i]=0;
    memcpy(LocalDir,LocalDir0,
      ((i=(int)(cc-(char far *)LocalDir0))>PATH_MAX)?PATH_MAX:i);
    if (chdir(LocalDir)==-1) {CurDirClu=SaveDir; return -1; }
    }
  return chdir(cc+1);
  }
else
  { /* short pathname (dirname only, without '/' separators) */
  h=open(LocalDir0,O_RDONLY);
  if (h==-1) return -1;
  if (!FCBS[h].Attr.B.Dir) {close(h); errno=ENOTDIR; return -1;}
  CurDirClu=FCBS[h].BegClu;
  }
close(h);
return 0;
}
/****************************************************************************/
DIR far *opendir (const char far *dirname)
{int h; DIR far *fp;

if ((h=open(dirname,O_RDONLY))==-1) return NULL;
if (!FCBS[h].Attr.B.Dir) {close(h); errno=ENOTDIR; return NULL;}
fp=FCBS;
fp+=h;
return fp;
}
/****************************************************************************/
DIR far *openwdir (const char far *dirname)
{int h; DIR far *fp;

if ((h=open(dirname,O_RDWR))==-1) return NULL;
if (!FCBS[h].Attr.B.Dir) {close(h); errno=ENOTDIR; return NULL;}
fp=FCBS;
fp+=h;
return fp;
}
/****************************************************************************/
DIR far *openidir (long inode)
{unsigned long OldCurDir;
DIR far *fp;
OldCurDir=CurDirClu;
CurDirClu=inode;
fp=openwdir(".");
CurDirClu=OldCurDir;
return fp;
}
/****************************************************************************/
struct dirent far *readdir (const DIR far *dirp)
{
long int i;
int h;
struct dirent16 far *D16;
struct dirent32 far *D32;
struct dirent far *D;
unsigned long first_clu;
unsigned long filesize;
struct ftime VremyaChe;

if (dirp==NULL) {errno=EBADF; return NULL;}
if((h=fileno(dirp))==-1) return NULL;

i=read(h,FCBS[h].Buf,32);
if (i!=32) return NULL;

if (RootBegClu)
  {
  /* FAT 32*/
  D = (void far *)(FCBS[h].Buf);
  D32 = (void far *)(FCBS[h].Buf);
  first_clu=((((unsigned long)(D32->FirstCluHi))<<16)|D32->FirstCluLo);
  filesize=D32->Size;

  VremyaChe.ft_tsec  =D32->UpdateDateTime.ft_tsec;
  VremyaChe.ft_min   =D32->UpdateDateTime.ft_min;
  VremyaChe.ft_hour  =D32->UpdateDateTime.ft_hour;
  VremyaChe.ft_day   =D32->UpdateDateTime.ft_day;
  VremyaChe.ft_month =D32->UpdateDateTime.ft_month;
  VremyaChe.ft_year  =D32->UpdateDateTime.ft_year;

  D->FirstClu=first_clu;
  D->Size=filesize;

  D->FileUpdateDateTime.ft_tsec=VremyaChe.ft_tsec;
  D->FileUpdateDateTime.ft_min=VremyaChe.ft_min;
  D->FileUpdateDateTime.ft_hour=VremyaChe.ft_hour;
  D->FileUpdateDateTime.ft_day=VremyaChe.ft_day;
  D->FileUpdateDateTime.ft_month=VremyaChe.ft_month;
  D->FileUpdateDateTime.ft_year=VremyaChe.ft_year;
  }
else
  { /* FAT-12/16 */
  D = (void far *)(FCBS[h].Buf);
  D16 = (void far *)(FCBS[h].Buf);
  first_clu=D16->d_fileno;
  filesize=D16->Size;

  VremyaChe.ft_tsec  =D16->FileDateTime.ft_tsec;
  VremyaChe.ft_min   =D16->FileDateTime.ft_min;
  VremyaChe.ft_hour  =D16->FileDateTime.ft_hour;
  VremyaChe.ft_day   =D16->FileDateTime.ft_day;
  VremyaChe.ft_month =D16->FileDateTime.ft_month;
  VremyaChe.ft_year  =D16->FileDateTime.ft_year;

  D->FirstClu=first_clu;
  D->Size=filesize;

  D->FileUpdateDateTime.ft_tsec=VremyaChe.ft_tsec;
  D->FileUpdateDateTime.ft_min=VremyaChe.ft_min;
  D->FileUpdateDateTime.ft_hour=VremyaChe.ft_hour;
  D->FileUpdateDateTime.ft_day=VremyaChe.ft_day;
  D->FileUpdateDateTime.ft_month=VremyaChe.ft_month;
  D->FileUpdateDateTime.ft_year=VremyaChe.ft_year;
  }
return (struct dirent far *)(FCBS[h].Buf);
}
/****************************************************************************/
int writedir (const DIR far *dirp, struct dirent far *D)
{
long int i;
int h;
if (dirp==NULL) {errno=EBADF; return -1;}
if((h=fileno(dirp))==-1) return -1;
i=write(h,D,32);
if (i!=32) return -1;
return 0;
}
/****************************************************************************/
void rewinddir (const DIR far *dirp)
{
lseek(fileno(dirp),0,SEEK_SET);
}
/****************************************************************************/
int closedir (const DIR far *dirp)
{
if (close(fileno(dirp))==-1) return EOF; else return 0;
}
/****************************************************************************/
long telldir (const DIR far *dirp)
{
return tell(fileno(dirp));
}
/****************************************************************************/
long seekdir (const DIR far *dirp, long offset, int wh)
{
return lseek(fileno(dirp),offset,wh);
}
/****************************************************************************/
int dircmp (struct dirent d1, struct dirent d2)
{
#ifdef DEBUG
  {int i;
  printf("dircmp: d1=<");
  for (i=0;i<11;i++) printf("%c",d1.d_name[i]);
  printf(">_d2=<");
  for (i=0;i<11;i++) printf("%c",d2.d_name[i]);
  printf("> ");
  }
#endif
if (d1.attrib.B.Attr1)
  {
  if (d2.attrib.B.Attr1)
    {
    return memcmp(d1.filename,d2.filename,11);
    }
  else
    {
    return -1;
    }
  }
else
  {
  if (d2.attrib.B.Attr1)
    {
    return -1;
    }
  else
    {
    return memcmp(d1.filename,d2.filename,11);
    }
  }
}
/****************************************************************************/
#pragma warn -par
int mkdir (const char far *path, mode_t mode)
{int i, j, h, err;
struct dirent Pattern [2];

if ((h=open(path,O_CREAT|O_EXCL|O_WRONLY))==-1) return -1 ;
FCBS[h].Attr.U=FA_DIREC;
FCBS[h].File=2;
for (i=0; i<11; i++) {Pattern[0].filename[i]=' '; Pattern[1].filename[i]=' '; }
Pattern[0].filename[0]='.';
Pattern[1].filename[0]='.';
Pattern[1].filename[1]='.';
Pattern[0].attrib.U=FA_DIREC;
Pattern[1].attrib.U=FA_DIREC;
Pattern[0].Size=0L;
Pattern[1].Size=0L;
Pattern[1].FirstClu=(unsigned int)CurDirClu;
Pattern[0].FileUpdateDateTime.ft_hour  = FCBS[h].FileDateTime.ft_hour  ;
Pattern[0].FileUpdateDateTime.ft_min   = FCBS[h].FileDateTime.ft_min   ;
Pattern[0].FileUpdateDateTime.ft_tsec  = FCBS[h].FileDateTime.ft_tsec  ;
Pattern[0].FileUpdateDateTime.ft_day   = FCBS[h].FileDateTime.ft_day   ;
Pattern[0].FileUpdateDateTime.ft_month = FCBS[h].FileDateTime.ft_month ;
Pattern[0].FileUpdateDateTime.ft_year  = FCBS[h].FileDateTime.ft_year  ;
Pattern[1].FileUpdateDateTime.ft_hour  = FCBS[h].FileDateTime.ft_hour  ;
Pattern[1].FileUpdateDateTime.ft_min   = FCBS[h].FileDateTime.ft_min   ;
Pattern[1].FileUpdateDateTime.ft_tsec  = FCBS[h].FileDateTime.ft_tsec  ;
Pattern[1].FileUpdateDateTime.ft_day   = FCBS[h].FileDateTime.ft_day   ;
Pattern[1].FileUpdateDateTime.ft_month = FCBS[h].FileDateTime.ft_month ;
Pattern[1].FileUpdateDateTime.ft_year  = FCBS[h].FileDateTime.ft_year  ;
if (write(h, Pattern, 2*32)==2*32) err=0; else {err=1; goto l_cl; }
Pattern[0].FirstClu=(unsigned int)FCBS[h].BegClu;
if (lseek(h,0,SEEK_SET)==-1L) return -1;
if (write(h, Pattern, 2*32)==2*32) err=0; else {err=1; goto l_cl; }
/*
Pattern[0].filename[0]=0xE5;
for(i=2;i<(CluSize*SECTOR_SIZE/32);i++)
  {
  if (write(h, Pattern, 32)==32) err=0; else {err=1; goto l_cl; }
  }
  */
l_cl:
if (close(h)==-1) err++;
return err?-1:0;
}
#pragma warn +par
/****************************************************************************/
int rmdir (const char far *path)
{int i; DIR far *dir; struct dirent far *D;
/* Проверка, непустой ли каталог */

if ((dir=opendir(path))==NULL) {errno=ENOENT; return -1;}
else
  {
  for(i=0;i<2;i++)
    {
    if ((D=readdir(dir))==NULL) {i=errno; closedir(dir); errno=i; return -1; }
    }

  for(;;)
    {
    if ((D=readdir(dir))==NULL) break;
    if ((*D).filename[0]==   0) continue;
    if ((*D).filename[0]==0xE5) continue;
    /* Hайден ненулевой вход - следовательно, каталог не пуст,
    следовательно его нельзя удалять */
    closedir(dir);
    errno=EEXIST;
    return -1;
    }
  if (closedir(dir)==-1) return -1;
  }

/* Удаление каталога */
return delete(path);
}