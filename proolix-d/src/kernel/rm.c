/*-------------------------------------------------------------------------*/
/*                                 internal rm                             */
/*-------------------------------------------------------------------------*/

#include <prool.h>

#include <conf.c>
#include "kernel.h"

void rm_help(void)
{
puts("Uses: rm [files...]");
}

void rm(int argc, char far *argv[] )
{
int ii, j, n, Files=0;

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
          default : rm_help();  return;
          }
      }
    else
      {
      Files=1;
      if ((unlink(argv[ii]))==-1) perror("rm");
      }
    }
  }
while (ii++<argc);

if (!Files) rm_help();

return ;
}
