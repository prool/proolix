#include <prool.h>

#include <conf.c>
#include "kernel.h"

void interrupt Int0(void)
{
puts("\nInterrupt 0 - IMHO, divide by zero\n");
#if 0
  loop();
#else
  getch();
#endif
}
