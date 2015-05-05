/*-------------------------------------------------------------------------*/
/*              external cp utility (like POSIX' cp)                       */
/*              for OS Proolix, MSDOS, Linux                               */
/*              by Serge Pustovoitoff (Prool), prool@itl.net.ua            */
/*              WWW http://prool.kharkov.org                               */
/*-------------------------------------------------------------------------*/

#ifdef Proolix
#include <conf.c>
#else
#define SECTOR_SIZE 512
#define PATH_MAX 128
#endif

#include <limits.h>

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#if defined Proolix || defined MSDOS
#include <io.h>
#endif

void cp_help(void)
{
puts("usage: cp [file1] [file2]");
}

int copy(char *path1, char *path2)
{int h1, h2, i1, i2; long n, m;
char buf [SECTOR_SIZE];

#if defined Proolix || defined Linux
if((h1=open(path1,O_RDONLY))==-1)
#else
if((h1=open(path1,O_RDONLY|O_BINARY))==-1)
#endif
  {
  printf("cp: can't open source\n");
  return -1;
  }

if (access(path2,2)==0)
  if (unlink(path2))
    {printf("cp: can't unlink target\n"); return -1; }

#if defined Proolix || defined Linux
if((h2=open(path2,O_WRONLY|O_CREAT|O_EXCL))==-1)
#else
if((h2=open(path2,O_WRONLY|O_CREAT|O_EXCL|O_BINARY,S_IWRITE))==-1)
#endif
  {
  printf("cp: can't open target\n");
  close(h1);
  return -1;
  }
while(1)
  {
  if((n=read(h1,buf,SECTOR_SIZE))==-1) break;
  if(n==0) {m=0; break;}
  if((m=write(h2,buf,n))==-1) break;
  if (m!=n) break;
  }
i1=close(h1);
i2=close(h2);
if (n==-1) {printf("cp: can't read source\n"); return -1;}
if (m==-1) {printf("cp: can't write target\n"); return -1;}
if (m!=n)  {printf("cp: write error (device full or not ready?)\n");
            return -1;}
if (i1==-1) {printf("cp: can't close source\n");return -1;}
if (i2==-1)
{
printf(
"cp: can't close target (device or directory full or device not ready)\n"
);
return -1;
}
return 0; /* OK! */
}

main(int argc, char *argv[] )
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
          default : cp_help();  return 1;
          }
      }
    else
      {
      strcpy(path[Files++],argv[ii]);
      if (Files>2) {cp_help(); return 3;}
      }
    }
  }
while (ii++<argc);

if (Files<2) {cp_help(); return 3;}
if ((copy(path[0],path[1]))==-1)
  {
  printf("Can't copy %s to %s\n",path[0],path[1]);
  return 1;
  }
return 0;
}