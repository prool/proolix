#include <prool.h>

#include <conf.c>
#include "kernel.h"

void new_video_page (char page)
{
  union REGS regs;
  regs.h.ah=5;
  regs.h.al=page;
  regs.x.flags=0;
  int86(0x10,&regs,&regs);
}
