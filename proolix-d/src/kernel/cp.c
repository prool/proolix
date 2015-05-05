/*-------------------------------------------------------------------------*/
/*                                 internal cp                             */
/*-------------------------------------------------------------------------*/

#include <prool.h>

#include <conf.c>
#include "kernel.h"

void cp_help(void)
{
puts("usage: cp [file1] [file2]");
}

int copy(char *path1, char *path2)
{int h1, h2, i1, i2;
long n, m;
char buf [SECTOR_SIZE];

if((h1=open(path1,O_RDONLY))==-1) return -1;

if (access(path2,2)==0)
  if (unlink(path2))
    {printf("Cp: can't unlink target\n"); return -1; }

if((h2=open(path2,O_WRONLY|O_CREAT|O_EXCL))==-1) {close(h1); return -1;}
while(1)
  {
  if((n=read(h1,buf,SECTOR_SIZE))==-1) break;
  if(n==0) {m=0; break;}
  if((m=write(h2,buf,n))==-1) break;
  if (m!=n) break;
  }
i1=close(h1);
i2=close(h2);
if (n==-1) return -1;
if (m==-1) return -1;
if (m!=n)  return -1;
if (i1==-1) return -1;
return (i2);
}

void cp(int argc, char far *argv[] )
{
int ii, j, n, Files=0;
char path[2][PATH_MAX];

argc--;

ii=1;
do
  {
  if (argc)
    {
    if (argv[ii][0]=='-')
      {
      n=(int)strlen(argv[ii]);
      for (j=1;j<n;j++)
        switch (argv[ii][j])
          {
          case 'N':
          default : cp_help();  return;
          }
      }
    else
      {
      strcpy(path[Files++],argv[ii]);
      if (Files>2) {cp_help(); return; }
      }
    }
  }
while (ii++<argc);

if (Files<2) {cp_help(); return; }
if ((copy(path[0],path[1]))==-1)
  {
  printf("Cp: Can't copy %s to %s\n",path[0],path[1]);
  return;
  }
return;
}
