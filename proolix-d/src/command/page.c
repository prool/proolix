#include <dos.h>
#include <stdio.h>
#include <stdlib.h>

void main(int argc, char *argv[])
{
union REGS regs;

if (argc==1)
  {
  regs.h.ah=15;
  regs.x.flags=0;
  int86(0x10,&regs,&regs);
  printf("Current videomode = %i, page = %i\n",regs.h.al,regs.h.bh);
  }
else
  {
  regs.h.ah=5;
  regs.h.al=atoi(argv[1]);
  regs.x.flags=0;
  int86(0x10,&regs,&regs);
  }
}
