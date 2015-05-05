#include <prool.h>
#include <stdio.h>

#include <conf.c>

#include "kernel.h"

#pragma warn-par
void stimer(int argc, char far *argv[])
{
#if 0
int i;

for(i=1;i<=argc;i++)
  {
  if (argv[i][0]=='-')
    switch (argv[i][1])
      {
      case 'y': Int8on=1; break;
      case 'n': Int8on=0; clrscr(); break;
      default: puts("usage `stimer -y` or `stimer -n`");
    }
  }
#else
puts("Timer not supported. Use `date` command");
#endif
}
#pragma warn+par