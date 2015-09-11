/*-------------------------------------------------------------------------*/
/*                                 internal mv                             */
/*-------------------------------------------------------------------------*/

#include <prool.h>

#include <conf.c>
#include "kernel.h"

void mv_help(void)
{
puts("Uses: mv [file1] [file2]");
}

void mv(int argc, char far *argv[] )
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
          default : mv_help();  return;
          }
      }
    else
      {
      strcpy(path[Files++],argv[ii]);
      if (Files>2) {mv_help(); return;}
      }
    }
  }
while (ii++<argc);

if (Files<2) {mv_help(); return;}
if ((rename(path[0],path[1]))==-1)
  {
  printf("Can't rename %s to %s\n",path[0],path[1]);
  return;
  }
return;
}
