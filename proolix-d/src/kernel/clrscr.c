#include <prool.h>

#include <conf.c>
#include "kernel.h"

extern void clrscr0(void);

void clrscr (void)
{
NLine=1;
clrscr0();
}
