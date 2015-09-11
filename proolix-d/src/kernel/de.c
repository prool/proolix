/*-------------------------------------------------------------------------*/
/*                                 internal disk edit                      */
/*-------------------------------------------------------------------------*/

#include <prool.h>

#include <conf.c>
#include "kernel.h"

void de (void)

{
unsigned int n;
char str[MAX_LEN_STR];

puts("Proolix Disk Editor by Serge Pustovoitoff\n\n\
The Disk Editor is currently in Read-Only mode");

printf("0 - boot sector\n1 -%i - FAT\n%i-%i - root dir\n%i-%i - data area\n",
RootBeg-1,RootBeg,DataStart-1,DataStart,MaxSectors-1);

while(1)
  {int line, adr, b;
  More=1; NLine=1;
  printf("Sector no > "); gets(str);
  *strchr(str,'\r')=0;
  if (!str[0]) break;
  n=atoi(str);
  printf("Sector no %i",n);
    if (n==0) puts(", boot sector");
    else if (n<RootBeg) puts(", FAT");
    else if (n<DataStart) puts(", root dir");
    else puts(", data area");
  if ((b=LoadCache(n))==-1) puts ("READ ERROR");
  else
    { /* */
    adr=0;
    for (line=0;line<32;line++)
      {int col;
      printf("%4X  ",adr);
      for (col=0;col<16;col++) printf("%2X ",*((*(Cache+b)).M+adr+col));
      for (col=0;col<16;col++)
        {char c;
        c=*((*(Cache+b)).M+adr+col);
        if (c>=' ') putch(c); else putch (' ');
        }
      puts("");
      adr+=16;
      }
    More=0; NLine=1;
    } /* */
  }
puts("");
ret:return;
}
