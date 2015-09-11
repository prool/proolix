#include <prool.h>

#include <conf.c>

#include "kernel.h"

void vec (int argc,char far *argv[])
{int i;
if (argc==2)
  {
  i=htoi(argv[1]);
  OutIntVector(i);
  return;
  }
else
  {
  More=2; NLine=1;
  for (i=0;i<256;i++)
    if (OutIntVector(i)==EOF) break;
  puts("");
  More=0; NLine=1;
  }
}
