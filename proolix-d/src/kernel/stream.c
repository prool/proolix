#include <prool.h>

#include <conf.c>
#include "kernel.h"
/****************************************************************************/
/* #define DEBUG */
/****************************************************************************/
int mflag(const char far *mode)
{int flag;
if (mode==NULL) return O_RDONLY;
if (*(mode+1)!='+')
  {
  switch (*mode)
    {
    case 'w': flag=O_WRONLY|O_CREAT|O_TRUNC; break;
    case 'a': flag=O_WRONLY|O_CREAT|O_TRUNC|O_APPEND; break;
    case 'r':
    default : flag=O_RDONLY;
    }
  }
else
  {
  switch (*mode)
    {
    case 'w': flag=O_RDWR|O_CREAT|O_TRUNC; break;
    case 'a': flag=O_RDWR|O_CREAT|O_TRUNC|O_APPEND; break;
    case 'r': flag=O_RDWR; break;
    default : flag=O_RDONLY;
    }
  }
return flag;
}
/****************************************************************************/
FILE far *fopen (const char far *path, const char far *mode)
{int h;
FILE far *fp;
if((h=open(path, mflag(mode)))==-1)return NULL;
fp=FCBS;
fp+=h;
return fp;
}
/****************************************************************************/
FILE far *freopen
(const char far *path, const char far *mode, FILE far *stream)
{
fclose(stream);
if(open2 (fileno(stream), path, mflag(mode))==-1)return NULL;
return stream;
}
/****************************************************************************/
FILE far* fdopen (int h, char far *type)
{FILE far *fp;
fp=FCBS;
fp+=h;
FCBS[h].flag=mflag(type);
return fp;
}
/****************************************************************************/
int fileno (const FILE far *stream)
{FILE far *fp;
fp=FCBS;
if (stream==NULL) return -1;
return (int)(stream-fp);
}
/****************************************************************************/
int fgetc (FILE far *stream)
{int h; char c;
h=fileno(stream);
if (h==-1) return EOF;
if (read(h,&c,1)==1) return c;
return EOF;
}
/****************************************************************************/
char far *_gets (char far *s)
{
#include "gets.inc"
}
/****************************************************************************/
char *fgets (char *s, int n, FILE far *stream)
{
#include "fgets.inc"
}
/****************************************************************************/
char far *_fgets (char far *s, int n, FILE far *stream)
{
#include "fgets.inc"
}
/****************************************************************************/
int fputc (int c, FILE far *stream)
{
switch((int)write(fileno(stream),&c,1))
  {
  case  1: return c;
  case  0: /* eof */
  case -1:
  default: return EOF;
  }
}
/****************************************************************************/
int fputs (const char far *s, FILE far *stream)
{
return (int)fwrite(s,1,strlen(s),stream);
}
/****************************************************************************/
size_t fread (void far *ptr, size_t size, size_t n, FILE far *stream)
{
long int i;
if ((size==0)||(n==0))return 0;
i=read(fileno(stream),ptr,size*n);
if (i>=0) return i/size;
return -1;
}
/****************************************************************************/
size_t fwrite (const void far *ptr, size_t size, size_t n, FILE far *stream)
{
long int i;
if ((size==0)||(n==0))return 0;
i=write(fileno(stream),ptr,size*n);
if (i>=0) return i/size;
return -1;
}
/****************************************************************************/
long ftell (FILE far *stream)
{
return tell(fileno(stream));
}
/****************************************************************************/
int fseek (FILE far *stream, long offset, int wh)
{
if (lseek(fileno(stream),offset,wh)==-1) return -1;
return 0;
}
/****************************************************************************/
int fsetpos (FILE far *stream, const fpos_t far *pos)
{
return fseek(stream, *pos, SEEK_SET);
}
/****************************************************************************/
int fgetpos (FILE far *stream, fpos_t far *pos)
{
if((*pos=ftell(stream))==-1)return -1;
return 0;
}
/****************************************************************************/
void rewind (FILE far *stream)
{
(void)fseek(stream,0L,SEEK_SET);
}
/****************************************************************************/
int fclose (FILE far *stream)
{
if (close(fileno(stream))==-1) return EOF; else return 0;
}
/****************************************************************************/
#pragma warn -par
int fflush (FILE far *stream)
{
return FlushAll();
}
#pragma warn +par
/****************************************************************************/
