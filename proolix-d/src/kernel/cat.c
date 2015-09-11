/*-------------------------------------------------------------------------*/
/*                                 internal cat                            */
/*-------------------------------------------------------------------------*/

/* #define DEBUG */

#include <prool.h>

#include <conf.c>
#include "kernel.h"

int CatLine;

void cat (int argc, char far *argv[] )
{
int h;
unsigned char c, far *cc;
int i, Files=0;

#ifdef DEBUG
printf("Cat: %s\n",argv[1]);
#endif

CatLine=0;

cc=&c;

argc--;

More=1; NLine=1;

i=1;
do
  {
  if (argc)
    {
      Files++;
      if ((h=open(argv[i],O_RDONLY))==-1)
        {
        printf("Can't open %s\n",argv[i]);
        }
      while (1)
        {
        if(read(h,cc,1)<=0) break;
        putch(c);
        }
      close(h);
    }
  }
while (i++<argc);

if (!Files)
  while(1)
    {
    c=getch();
    if (c==4) break; /* 4 - CTRL-D */
    else putch(c);
    }

More=0; NLine=1;

return;
}
