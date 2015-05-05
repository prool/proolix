#include <prool.h>

#include <conf.c>
#include "kernel.h"

#include <stdarg.h>
#include <sys/types.h>
/****************************************************************************/
#define CheckUserWriteDir
/* #define DEBUG */
/****************************************************************************/
int FlushBuf(unsigned int i)
{
if ((*(Cache+i)).Status==2)
  {
  #if 0
  printf(" w");
  #endif
  if (abswrite(TR(CurrentDevice),1,(*(Cache+i)).Sec,(*(Cache+i)).M))
    {
    printf("\nFlushBuf: abswrite error on sector %i\n",(*(Cache+i)).Sec);
    return -1;
    }
  else
    {
    (*(Cache+i)).Status=1;
    }
  }
return 0;
}
/****************************************************************************/
int FlushAll(void)
{int i, err;
err=0;
for (i=0;i<BUFFERS;i++) err+=FlushBuf(i);
if (err) return -1;
else return 0;
}
/****************************************************************************/
void TouchCache(int n)
{int i; /* почитай комментарий в начале функции NewCache */
for(i=0;i<BUFFERS;i++)
  if (i==n) (*(Cache+i)).Counter=0;
  else (*(Cache+i)).Counter++;
}
/****************************************************************************/
int NewCache (unsigned long sec)
{/* почитай комментарий в начале функции LoadCache */
int i, j, max, maxbuf;
for (i=0;i<BUFFERS;i++)
  /* если требуемый сектор найден в буферном пуле, то игнорируем его значение
  */
  if (((*(Cache+i)).Sec==sec)&&((*(Cache+i)).Status!=0))
    goto Fill;
/* сектор в пуле не найден :( */
/* ищем свободный буфер */
for (i=0;i<BUFFERS;i++)
  if ((*(Cache+i)).Status==0) goto Fill;
/* свободных буферов нет. Ищем кандидата на освобождение */
max=(*Cache).Counter;
maxbuf=0;
for (i=1;i<BUFFERS;i++)
  if ((*(Cache+i)).Counter>max)
    {
    max=(*(Cache+i)).Counter;
    maxbuf=i;
    }
/* нашли кандидата */
i=maxbuf;
if (FlushBuf(i)) return -1;
Fill:
/* исписываем сектор нулями */
(*(Cache+i)).Status=2;
for (j=0;j<SECTOR_SIZE;j++) (*(Cache+i)).M[j]=0;
(*(Cache+i)).Sec=sec;
TouchCache(i);
return i;
}
/****************************************************************************/
int LoadCache (unsigned long sec)
{
/*
алгоритм освобождения буферов, когда все буфера
заполнены, а требуется еще один: надо освобождать буфер, обращение к
которому было самым последним (см. книгу [Бах])
только у меня это сделано путем прибавления счетчиков в буферах (см.
функцию TouchCache(), а надо бы сделать список, как у Баха, только без
хеш-очередей (для простоты)
*/
int i, max, maxbuf;

for (i=0;i<BUFFERS;i++)
  /* если требуемый сектор найден в буферном пуле,возвращаем его номер */
  if ( ((*(Cache+i)).Sec==sec) &&
       ( ((*(Cache+i)).Status==1) || ((*(Cache+i)).Status==2) )  )
    {
    TouchCache(i);
    return i;
    }
/* сектор в пуле не найден :( */
/* ищем свободный буфер */

for (i=0;i<BUFFERS;i++)
  if ((*(Cache+i)).Status==0) goto Load;
/* свободных буферов нет. Ищем кандидата на освобождение */

max=(*Cache).Counter;
maxbuf=0;
for (i=1;i<BUFFERS;i++)
  if ((*(Cache+i)).Counter>max)
    {
    max=(*(Cache+i)).Counter;
    maxbuf=i;
    }
/* нашли кандидата */

i=maxbuf;
if (FlushBuf(i)) return -1;

Load:
if (absread(TR(CurrentDevice), 1, sec, (*(Cache+i)).M ))
  {
  #if 0
  printf("\nLoadCache: absread error on sector %i\n",sec);
  #endif
  (*(Cache+i)).Status=0;
  return -1;
  }

(*(Cache+i)).Status=1;
(*(Cache+i)).Sec=sec;

TouchCache(i);

return i;
}
struct dirent Bolvan;
/****************************************************************************/
int open2 (int h, const char far *path, int flag)
{unsigned long  i;
unsigned long j;
int ii;
int Search, File, DevNo;
unsigned long SaveCur;
struct dirent far *D;
struct time timep;
struct date datep;
DIR far *dir;
char far *cc;
char far *cc0;
const char far *name;
char path0 [PATH_MAX+1];
struct dirent *pBolvan;

pBolvan=&Bolvan;

/* специальный файл ? */
ii=0;
File=1;
if (!memcmp(path,"/dev/",5))
  {
  do
    {
    if (!strcmp(path+5,Dev[ii++].Name)) {File=0; DevNo=ii; break;}
    }
  while (Dev[ii].Name[0]);
  }
if (!File) FCBS[h].File=0; else FCBS[h].File=1;

/* Разбор имени path на собственно-path (path0) и name */
strncpy(path0,path,PATH_MAX); path0[PATH_MAX]=0;
#if 0
/* Если в path0 последний символ '/', и он не один, то загасим его */
i=strlen(path0);
if (i-->1) if (path0[i]=='/') path0[i]=0;
#endif

cc=path0; cc0=NULL;
while ((cc=strchr(cc,'/'))!=NULL)
  cc0=cc++;
if (cc0==NULL)
  {
  /* В пути path нет ни одного символа '/',
     что означает, что это локальное имя (без пути) */
  path0[0]=0;
  name=path;
  }
else
  {
  /* Имя path содержит по крайней мере один символ '/',
     значит это имя с путем. ("Все путем" ;) */
  /* Запоминаем текущий каталог */
  SaveCur=CurDirClu;
  if (cc0==path0)
    {/* символ '/' первый и единственный (имя вида "/file") */
    name=path0+1;
    /* Делаем текущим каталог "/" */
    if (chdir("/")==-1)
      {
      #ifdef DEBUG
      printf("L1");
      #endif
      errno=ENOENT;
      goto ret_err;
      }
    }
  else
    {
    *cc0++=0;
    name=cc0;
    /* Делаем текущим каталог path0 */
    if (chdir(path0)==-1)
      {
      #ifdef DEBUG
      printf("L2");
      #endif
      errno=ENOENT;
      goto ret_err;
      }
    }
  }

/* Если открываем файл "/" или при текущем корневом каталоге открываем "."
   или "..", то значит, открываем root dir */

if ( ( strcmp(path,"/")==0 ) ||
     ( (CurDirClu==0) && ( (!strcmp(name,"."))||(!strcmp(name,"..")) ) )
   )
  {/* open (rootdir) */
  if (RootBegClu) /* RootBegClu>0 только при смонтированной ФС FAT-32 или FAT32LBA*/
    FCBS[h].File=2;
  else
    FCBS[h].File=3;
    #ifdef CheckUserWriteDir
    if (((flag&O_WRONLY)||(flag&O_RDWR)) && Mode) {errno=EACCES; goto ret_err;}
    #endif
  strcpy(FCBS[h].Name,"/");
  FCBS[h].BegClu=RootBegClu;
  FCBS[h].Attr.B.Dir=1;
  FCBS[h].flag=flag;
  FCBS[h].Size=-1;
  FCBS[h].AbsOffset=0l;
  FCBS[h].AbsSec= RootBegClu ? SecForClu(RootBegClu) : RootBeg;
  goto ret_norm;
  }

if (!strcmp(name,".")) /* файл "." (текущий каталог) должен открываться
специальным способом, с использованием хранящейся в системе информации о
текущем каталоге (переменная CurDirClu) */
  {/* open (curdir) */
  FCBS[h].File=2;
    #ifdef CheckUserWriteDir
    if (((flag&O_WRONLY)||(flag&O_RDWR)) && Mode)
      {
      errno=EACCES;
      return -1;
      }
    #endif
  strcpy(FCBS[h].Name,name);
  FCBS[h].BegClu=CurDirClu;
  FCBS[h].Attr.B.Dir=1;
  FCBS[h].DirInode=-1;
  FCBS[h].flag=flag;
  FCBS[h].Size=-1;
  FCBS[h].AbsOffset=0l;
  FCBS[h].AbsSec=SecForClu(CurDirClu);
  FCBS[h].AbsClu=CurDirClu;
  goto ret_norm;
  }

if (File!=0)
  {/* file or dir */
  /*  Ищем файл (or dir) в каталоге */
  PathToDir(name, pBolvan);
  #ifdef DEBUG
  printf("open2: `%Fs'->",name);
  printf("`%s' ",Bolvan.d_name);
  #endif
  if ((dir=opendir("."))==NULL)
    {puts("open: can't search in current dir"); errno=EFAULT; goto ret_err;}
  while(1)
      {
      if ((D=readdir(dir))==NULL) break;
      if ( (*D).filename[0]==0    ) continue;
      if ( (*D).filename[0]==0xe5 ) continue;
      if ( (*D).attrib.B.Label    ) continue;
      #ifdef DEBUG
      /* printf("open2: d1=`%s' d2=`%s' ",(*D).d_name,Bolvan.d_name); */
      #endif
      if (dircmp((*D),Bolvan)==0)
        {Search=1;
        FCBS[h].DirInode=CurDirClu;
        FCBS[h].DirSeekPos=telldir(dir)-32;
        goto l_cont; }
      }
  Search=0;
  l_cont:
  if (closedir(dir)==-1) goto ret_err;
  if (Search)
    {
    if ((flag&O_EXCL)&&(flag&O_CREAT)) {errno=EEXIST; goto ret_err;}
    FCBS[h].Attr.U=(*D).attrib.U;
    if ((*D).attrib.B.Dir)
      {/* found dir */
        #ifdef CheckUserWriteDir
        if (((flag&O_WRONLY)||(flag&O_RDWR)) && User)
          {errno=EACCES; goto ret_err;}
        #endif
      FCBS[h].File=2;
      FCBS[h].Size=-1;
      }
    else
      {/* found ordinary (regular) file (not directory) */
      if (flag&O_TRUNC)
        { /* truncate */
        if ((i=(*D).FirstClu)!=0)
        while(1)
          {
          j=NextClu(i);
          if(LinkClu(i,0)==-1) {errno=EFAULT; goto ret_err;};
          if (j==-1) break;
          else i=j;
          }
        (*D).Size=0;
        (*D).FirstClu=0;
        #if 0
        if ((dir=openwdir("."))==NULL)
          {puts("open: can't search in current dir"); errno=EFAULT; goto ret_err;}
        if (seekdir(dir,FCBS[h].DirSeekPos,SEEK_SET)==-1) goto ret_err;
        if (writedir(dir,D)==-1) goto ret_err;
        if (closedir(dir)==-1) goto ret_err;
        #endif
        }
      FCBS[h].Size=(*D).Size;
      }
    FCBS[h].flag=flag;
    FCBS[h].FileDateTime.ft_hour =(*D).FileUpdateDateTime.ft_hour;
    FCBS[h].FileDateTime.ft_min  =(*D).FileUpdateDateTime.ft_min ;
    FCBS[h].FileDateTime.ft_tsec =(*D).FileUpdateDateTime.ft_tsec ;
    FCBS[h].FileDateTime.ft_day  =(*D).FileUpdateDateTime.ft_day  ;
    FCBS[h].FileDateTime.ft_month=(*D).FileUpdateDateTime.ft_month;
    FCBS[h].FileDateTime.ft_year =(*D).FileUpdateDateTime.ft_year ;
    if ((FCBS[h].BegClu=(*D).FirstClu)==0)
      {
      FCBS[h].AbsSec=RootBeg;
      FCBS[h].File=3;
      }
    else
      {
      FCBS[h].AbsClu=FCBS[h].BegClu;
      FCBS[h].AbsSec=SecForClu(FCBS[h].AbsClu);
      }
    FCBS[h].AbsOffset=0l;
    }
  else
    { /* File not exist*/
    if (flag&O_RDONLY)
      {
      #ifdef DEBUG
      printf("L3");
      #endif
      errno=ENOENT;
      goto ret_err;
      }
    /* Creating */
    /* Ищем свободный вход в каталоге */
    if ((dir=openwdir("."))==NULL)
      {puts("open: can't search in current dir"); errno=EFAULT; goto ret_err;}
    while(1)
      {
      if ((D=(void far *)readdir(dir))==NULL) break;
      if (!(*D).filename[0] || ((*D).filename[0]==0xe5))
        {/* нашли св. вход в каталог */
        /* создаем запись о файле */
        /* заполняем поля FCB */
        FCBS[h].DirInode=CurDirClu;
        if ((FCBS[h].DirSeekPos=seekdir(dir,-32l,SEEK_CUR))==-1)
          printf("open: can't seek cur dir");
        (*D).attrib.U=FA_ARCH;
        PathToDir(name,D);
        gettime (&timep);
          (*D).FileUpdateDateTime.ft_tsec=timep.ti_sec*2;
          (*D).FileUpdateDateTime.ft_min =timep.ti_min;
          (*D).FileUpdateDateTime.ft_hour=timep.ti_hour;
        getdate(&datep);
          (*D).FileUpdateDateTime.ft_day  =datep.da_day;
          (*D).FileUpdateDateTime.ft_month=datep.da_mon;
          (*D).FileUpdateDateTime.ft_year =datep.da_year;
        (*D).FirstClu=0;
        (*D).Size=0;
        /* заполняем поля FCB */
        FCBS[h].Attr.U   =(*D).attrib.U;
        FCBS[h].flag=flag;
        FCBS[h].FileDateTime.ft_hour =(*D).FileUpdateDateTime.ft_hour;
        FCBS[h].FileDateTime.ft_min  =(*D).FileUpdateDateTime.ft_min ;
        FCBS[h].FileDateTime.ft_tsec =(*D).FileUpdateDateTime.ft_tsec ;
        FCBS[h].FileDateTime.ft_day  =(*D).FileUpdateDateTime.ft_day  ;
        FCBS[h].FileDateTime.ft_month=(*D).FileUpdateDateTime.ft_month;
        FCBS[h].FileDateTime.ft_year =(*D).FileUpdateDateTime.ft_year ;
        FCBS[h].BegClu=(*D).FirstClu;
        FCBS[h].Size=(*D).Size;

        FCBS[h].AbsOffset=0l;
        FCBS[h].AbsClu=FCBS[h].BegClu;
        FCBS[h].AbsSec=0;
        if (writedir(dir, D)==0)
          {
          closedir(dir);
          goto ret_norm;
          }
        else goto L1;
        }
      }
    L1:
    /* св. входа нет :( */
    closedir(dir);
    errno=ENOSPC;
    goto ret_err;
    } /* file not exist. creating */
  } /* file */
else
  { /* dev */
  /* DRIVER /dev/?????????? */
  /*
  FCBS[h].Major=Dev[DevNo].Major;
  FCBS[h].Minor=Dev[DevNo].Minor;
  */
  FCBS[h].Size=-1;
  if (!Dev[DevNo].Byte)
    {
    FCBS[h].AbsOffset=0l;
    FCBS[h].AbsSec=0;
    }
  } /* dev */
ret_norm:
errno=0;
/* Если это необходимо, восстанавливаем прежний текущий каталог */
if (path0[0]) CurDirClu=SaveCur;
return h;

ret_err: FCBS[h].Name[0]=0;
/* Если это необходимо, восстанавливаем прежний текущий каталог */
if (path0[0]) CurDirClu=SaveCur;
return -1;
}
/****************************************************************************/
int open (const char far *path, int flag,...)
{int h, k;
/*
                                        Эти флаги реализованы
O_RDONLY
O_WRONLY
O_RDWR
O_CREAT        /* create and open file */
O_TRUNC        /* open with truncation */
O_EXCL         /* exclusive open */
O_APPEND       /* to end of file */
                                        Эти флаги будут реализованы?
O_NOINHERIT
O_DENYALL
O_DENYWRITE
O_DENYREAD
O_DENYNONE
                                        А нафига?
*/

/* Проверяем флаги на корректность */

/* Проверяем обязательные флаги на присуствие */

if (!((O_RDONLY|O_WRONLY|O_RDWR) & flag)) goto L_INVAL;

/* Проверяем флаги на несовместимые сочетания */

if ((flag&O_RDONLY)&&((flag&O_WRONLY)||(flag&O_RDWR))) goto L_INVAL;
if ((flag&O_RDWR)&&((flag&O_WRONLY))) goto L_INVAL;
if ((flag&O_TRUNC)&&((flag&O_RDONLY))) goto L_INVAL;

h=-1;

if (path==NULL)
  {
  goto L_INVAL;
  }
if (*path==0)
  {
  L_INVAL:
  #ifdef DEBUG
  printf("L4");
  #endif
  errno=ENOENT;
  return -1;
  }

/* Здесь (или далее) сделать сравнение открываемого файла с уже открытыми
по номеру нач. кластера и номеру файловой системы */
/*
for(k=0;k<OPEN_MAX;k++)
  if (FCBS[k].Name[0])
    if (!strcmp(path,FCBS[k].Name))
      {
      errno=EACCES;
      return -1;
      }
      */

/* Выделение свободного хандлера */
for (k=0;k<OPEN_MAX;k++)
  if (!(FCBS[k].Name[0]))
    {strncpy(FCBS[k].Name,path,NAME_MAX+1); h=k; break;}

if (h==-1)
  {
  errno=EMFILE;
  return -1;
  }

if (open2(h, path, flag)==-1)
  {
  return -1;
  }
return h;
}
/****************************************************************************/
int creat (const char far *path, mode_t mode)
{
return open(path,O_WRONLY|O_CREAT|O_TRUNC,mode);
}
/****************************************************************************/
int getftime (int h, struct ftime far *ftimep)
{
if ((h<0)||(h>OPEN_MAX)) {errno=EBADF; return -1;}
if (ftimep==NULL) {errno=EINVAL; return -1;}
if (!FCBS[h].Name[0]) {errno=EBADF; return -1;}

(*ftimep).ft_hour = FCBS[h].FileDateTime.ft_hour  ;
(*ftimep).ft_min  = FCBS[h].FileDateTime.ft_min   ;
(*ftimep).ft_tsec = FCBS[h].FileDateTime.ft_tsec  ;
(*ftimep).ft_day  = FCBS[h].FileDateTime.ft_day   ;
(*ftimep).ft_month= FCBS[h].FileDateTime.ft_month ;
(*ftimep).ft_year = FCBS[h].FileDateTime.ft_year  ;

return 0;
}
/****************************************************************************/
int setftime (int h, struct ftime far *ftimep)
{DIR far *fp; struct dirent far *D;
if ((h<0)||(h>OPEN_MAX)) {errno=EBADF; return -1;}
if (ftimep==NULL) {errno=EINVAL; return -1;}
if (!FCBS[h].Name[0]) {errno=EBADF; return -1;}

if ( (FCBS[h].flag & O_WRONLY) || (FCBS[h].flag & O_RDWR) )
  {
  if ((fp=openidir(FCBS[h].DirInode))==NULL) return -1;
  if (seekdir(fp,FCBS[h].DirSeekPos,SEEK_SET)==-1) return -1;
  if ((D=readdir(fp))==NULL) return -1;
  if (seekdir(fp,FCBS[h].DirSeekPos,SEEK_SET)==-1) return -1;
  (*D).FileUpdateDateTime.ft_hour  = (*ftimep).ft_hour    ;
  (*D).FileUpdateDateTime.ft_min   = (*ftimep).ft_min     ;
  (*D).FileUpdateDateTime.ft_tsec  = (*ftimep).ft_tsec    ;
  (*D).FileUpdateDateTime.ft_day   = (*ftimep).ft_day     ;
  (*D).FileUpdateDateTime.ft_month = (*ftimep).ft_month   ;
  (*D).FileUpdateDateTime.ft_year  = (*ftimep).ft_year    ;
  FCBS[h].FileDateTime.ft_hour  = (*ftimep).ft_hour ;
  FCBS[h].FileDateTime.ft_min   = (*ftimep).ft_min  ;
  FCBS[h].FileDateTime.ft_tsec  = (*ftimep).ft_tsec ;
  FCBS[h].FileDateTime.ft_day   = (*ftimep).ft_day  ;
  FCBS[h].FileDateTime.ft_month = (*ftimep).ft_month;
  FCBS[h].FileDateTime.ft_year  = (*ftimep).ft_year ;
  if (writedir(fp,D)==-1) return -1;
  return closedir(fp);
  }
else
  {
  errno=EPERM;
  }
return -1;
}
/****************************************************************************/
int chmod(const char far *filename, int amode)
{int h;
h=amode;
if ((h=open(filename,O_RDONLY))==-1) return -1;
else close(h);
return 0;
}
/****************************************************************************/
int _chmod(const char far *filename, int func, ...)
{int h; int attribute;
va_list v;

/* func: 0 - read attr, 1 - set attr. attr see in <dos.h> */

if ((h=open(filename,func?O_WRONLY:O_RDONLY))==-1) return -1;

if (func==1)
  {
  /* Функция 1 - установить атрибуты файла */
  int Label;
  int Dir;
  int Attr1;

  va_start(v,0);
  attribute = va_arg(v,int);

  /* Сохраняем неизменяемые атрибуты */
  Label=FCBS[h].Attr.B.Label;
  Dir  =FCBS[h].Attr.B.Dir  ;
  Attr1=FCBS[h].Attr.B.Attr1;

  /* Устанавливаем атрибуты */
  FCBS[h].Attr.U = attribute;

  /* Восстанавливаем неизменяемые атрибуты */
  FCBS[h].Attr.B.Label=Label;
  FCBS[h].Attr.B.Dir  =Dir  ;
  FCBS[h].Attr.B.Attr1=Attr1;
  }
else
  {/* Функция 0 - взять атрибуты файла */
  attribute=FCBS[h].Attr.U;
  }
if (close(h)==-1) return -1;
return attribute;
}
/****************************************************************************/
int access(const char far *filename, int amode)
{int flag, h;
if (amode==2) flag=O_WRONLY;
else flag=O_RDONLY;
if((h=open(filename,flag))==-1) return -1;
else if(close(h)==-1) return -1;
return 0;
}
/****************************************************************************/
long int read(int h, void far *buf, unsigned long len)
{
int i, b;
long l;
char c;

if ((h<0)||(h>OPEN_MAX)) {errno=EBADF; return -1;}
if (buf==NULL) {errno=EINVAL; return -1;}
if (len==0) {errno=0; return 0;}
if (!FCBS[h].Name[0]) {errno=EBADF; return -1;}
if (FCBS[h].flag & O_WRONLY) {errno=EACCES; return -1;}

if (h==0) if (FCBS[h].File==0)
  {
  for(i=0;i<len;i++)
    {
    *((char huge *)buf)++=getche();
    }
  errno=0;
  return len;
  }

if (FCBS[h].AbsOffset % SECTOR_SIZE == 0)
  {
  if (FCBS[h].AbsOffset != 0)
    {
    /* читаем след. сектор файла */
    if (FCBS[h].File==3)
      {/* это каталог и притом корневой */
      /* работу с устройствами типа /dev/fd0 или /dev/hd0 можно сделать аналогично */
      if (FCBS[h].AbsSec==RootEnd) {errno=0; return 0;}
      else FCBS[h].AbsSec++;
      }
    else
      {/* это обычный файл или некорневой каталог, который аналогичен обычному
      файлу */
      if (FCBS[h].AbsOffset % CluSizeBytes != 0)
        FCBS[h].AbsSec++;
      else
        {unsigned long ll;
        ll=NextClu(FCBS[h].AbsClu);
        FCBS[h].AbsClu=ll;
        if ((ll==0L)||(ll==0xffffffffL))
          {FCBS[h].AbsSec=-1; errno=0; return 0;}
        else
          FCBS[h].AbsSec=SecForClu(FCBS[h].AbsClu);
        }
      }
    }
  if ((b=LoadCache(FCBS[h].AbsSec))==-1) {errno=EFAULT; return -1;}
  }

/* Если чтение не требует подкачки в кеш нового сектора, то чтение
заключается только в перезаписи из кеша в буфер buf и соотв. сдвиге
указателя (головки) файла */

if ((FCBS[h].AbsOffset % SECTOR_SIZE + len) <= SECTOR_SIZE)
  {int SecOffset;
  if ((FCBS[h].AbsOffset+len) > FCBS[h].Size)
    if ((len=FCBS[h].Size-FCBS[h].AbsOffset)==0) {errno=0; return 0; }
  if ((b=LoadCache(FCBS[h].AbsSec))==-1) {errno=EFAULT; return -1; }
  SecOffset=(int)(FCBS[h].AbsOffset % SECTOR_SIZE);
  for (i=0;i<len;i++)
    *((char huge *)buf)++ = *((*(Cache+b)).M + SecOffset++ );
  FCBS[h].AbsOffset+=len;
  return len;
  }

i=SECTOR_SIZE-(int)(FCBS[h].AbsOffset % SECTOR_SIZE);
if (i==0) i=SECTOR_SIZE;
l=read(h,buf,i);
if (l==-1) return -1;
if (l<i) return l;
buf=(char far *)((char huge *)buf+i);

l=read(h,buf,len-i);
if (l==-1) return -1;
return l+i;
}
/****************************************************************************/
long int write(int h, void far *buf, unsigned long len)
{
int b;
long i, l;
char c;

#ifdef DEBUG
printf("write: h=%i",h);
printf(" len=%lu\n",len);
#endif

if ((h<0)||(h>OPEN_MAX)) {errno=EBADF; return -1;}
if (buf==NULL) {errno=EINVAL; return -1;}
if (!FCBS[h].Name[0]) {errno=EBADF; return -1;}
if (FCBS[h].flag & O_RDONLY) {errno=EACCES; return -1;}
if (len==0) {errno=0; return 0;}

if((h>=0)&&(h<=2)) if (FCBS[h].File==0)
  {
  for(l=0;l<len;l++)
    {
    putch(*((char huge *)buf)++);
    }
  errno=0;
  return len;
  }

/* Если у файла еще нет кластеров, создаем начало цепочки */
/* Спец.файлам и корневому каталогу этого не делаем */

if ( ((FCBS[h].File==1)||(FCBS[h].File==2)) && (FCBS[h].BegClu==0) )
  {
  if ((i=GetFreeClu())==-1L) {errno=ENOSPC; return -1;}
  FCBS[h].BegClu=i;
  FCBS[h].AbsClu=i;
  FCBS[h].AbsSec=SecForClu(i);
  FCBS[h].AbsOffset=0;
  /* Прописываем нулями первый кластер файла */
  for (i=0;i<CluSize;i++)
    if (NewCache(FCBS[h].AbsSec+i)==-1) {errno=EFAULT; return -1;}
  }

if (FCBS[h].flag & O_APPEND) if(lseek (h,0,SEEK_END)==-1L) return -1;

if (FCBS[h].AbsOffset % SECTOR_SIZE == 0)
  {
  if (FCBS[h].AbsOffset != 0)
    {
    /* подкачиваем следующий сектор, если необходимо */
    if (FCBS[h].File==3)
      {/* это каталог и притом корневой */
      /* работу с устройствами типа /dev/fd0 или /dev/hd0 можно сделать аналогично */
      if (FCBS[h].AbsSec==RootEnd) {errno=0; return 0;}
      else FCBS[h].AbsSec++;
      }
    else if (FCBS[h].AbsOffset % CluSizeBytes != 0)
      FCBS[h].AbsSec++;
    else if ((i=NextClu(FCBS[h].AbsClu))==-1)
      {/* запись за пределы конца файла, удлиняем файл еще на 1 кластер */
      if((i=AppendClu(FCBS[h].AbsClu))==-1) {errno=ENOSPC; return -1;}
      FCBS[h].AbsClu=i;
      FCBS[h].AbsSec=SecForClu(FCBS[h].AbsClu);
      /* Прописываем нулями новый кластер файла */
      for (i=1;i<CluSize;i++)
        if (NewCache(FCBS[h].AbsSec+i)==-1) {errno=EFAULT; return -1;}
      if ((b=NewCache(FCBS[h].AbsSec))==-1) {errno=EFAULT; return -1;}
      }
    else
      {/* подкачиваем еще один кластер */
      FCBS[h].AbsClu=i;
      FCBS[h].AbsSec=SecForClu(FCBS[h].AbsClu);
      if ((b=LoadCache(FCBS[h].AbsSec))==-1) {errno=EFAULT; return -1;}
      }
    }
  }

/* Если запись не требует подкачки в кеш нового сектора, то запись
заключается только в перезаписи из буфера buf в кеш и соотв. сдвиге
указателя (головки) файла. */

if((FCBS[h].AbsOffset % SECTOR_SIZE+len) <= SECTOR_SIZE)
  {int SecOffset;
  if ((b=LoadCache(FCBS[h].AbsSec))==-1) {errno=EFAULT; return -1;}
  (*(Cache+b)).Status=2;
  SecOffset=(int)(FCBS[h].AbsOffset % SECTOR_SIZE);
  for (i=0;i<len;i++)
    *((*(Cache+b)).M + SecOffset++ ) = *((char huge *)buf)++;
  FCBS[h].AbsOffset+=len;
  return len;
  }
  /* В противном случае не мудрствуя лукаво len раз вызываем write с длиной 1
     (здесь потом можно вставить тот же код, что и на соотв. месте в read()
   */
for (i=0;i<len;i++)
  {
  c=*((char huge *)buf)++;
  l=write(h,&c,1);
  if (l==-1) return -1;
  if (!l) break;
  }
return i;
}
/****************************************************************************/
int close (int h)
{int Update=0;
long i;
struct dirent far *D; DIR far *fp;

if ((h<0)||(h>OPEN_MAX)) {errno=EBADF; return -1;}
else
   {
   if (FCBS[h].Name[0])
      {
      if ( (FCBS[h].File==1) || (FCBS[h].File==2) )
         {
         if ((FCBS[h].flag&O_WRONLY)||(FCBS[h].flag&O_RDWR))
            {
            if ((i=FCBS[h].DirInode)!=-1)
              {
              if ((fp=openidir(i))==NULL) return -1;
              if (seekdir(fp,FCBS[h].DirSeekPos,SEEK_SET)==-1) return -1;
              if ((D=readdir(fp))==NULL) return -1;
              if ((*D).FirstClu!=FCBS[h].BegClu)
                 {Update=1; (*D).FirstClu=(unsigned int)FCBS[h].BegClu;}
              if (FCBS[h].File==1) if ((*D).Size<FCBS[h].AbsOffset)
                 {Update=1; (*D).Size=FCBS[h].AbsOffset;}
              if ((*D).attrib.U!=FCBS[h].Attr.U)
                 {Update=1; (*D).attrib.U=FCBS[h].Attr.U;}
              if (Update)
                 {
                 if (seekdir(fp,FCBS[h].DirSeekPos,SEEK_SET)==-1) return -1;
                 if (writedir(fp,D)==-1) return -1;
                 }
              if (closedir(fp)==-1) return -1;
              }
            }
         }
      FCBS[h].Name[0]=0;
      }
   else return -1;
   }
return 0;
}
/****************************************************************************/
long int filelength (int h)
{
if ((h<0)||(h>OPEN_MAX)) {errno=EBADF; return -1;}
if(FCBS[h].Name[0]) return FCBS[h].Size;
else return -1;
}
/****************************************************************************/
long tell(int h)
{
if ((h<0)||(h>OPEN_MAX)) {errno=EBADF; return -1L;}
if (!FCBS[h].Name[0]) {errno=EBADF; return -1L;}
return FCBS[h].AbsOffset;
}
/****************************************************************************/
long lseek(int h, long offset, int fromwhere)
{int i;
long j, n;
if ((h<0)||(h>OPEN_MAX)) {errno=EBADF; return -1L;}
if (!FCBS[h].Name[0]) {errno=EBADF; return -1L;}
switch (fromwhere)
  {
  case SEEK_SET:
    if (offset<0) {errno=EINVAL; return -1L;}
    /* offset>=0 */
    /* установка файла в начало (rewind) */
    FCBS[h].AbsOffset=0l;
    if ((FCBS[h].File==1)||(FCBS[h].File==2)) /* file or directory */
      {
      FCBS[h].AbsClu=FCBS[h].BegClu;
      FCBS[h].AbsSec=SecForClu(FCBS[h].AbsClu);
      }
    else if (FCBS[h].File==3) /* root dir */
      FCBS[h].AbsSec=RootBeg;
    /* после этого мы перескакиваем на следующий пункт case и его выполнение
    делает именно то, что надо */
  case SEEK_CUR:
    if (offset==0) return FCBS[h].AbsOffset;
    if (offset>0)
      {char buf[SECTOR_SIZE];
      for (i=0; i<offset/SECTOR_SIZE; i++)
        {
        n=read(h,buf,SECTOR_SIZE);
        if (n!=SECTOR_SIZE) goto L_FAULT;
        }
      n=read(h,buf,j=offset%SECTOR_SIZE);
      if (n!=j) {L_FAULT: errno=EFAULT; return -1L;}
      return FCBS[h].AbsOffset;
      }
    else /* offset<0 */
      return lseek(h,FCBS[h].AbsOffset+offset,SEEK_SET);
  case SEEK_END:
  default:
    return lseek(h,FCBS[h].Size+offset,SEEK_SET);
  }
}
/****************************************************************************/
#pragma warn -par
int isatty(int h)
{
return 0;
}
#pragma warn +par