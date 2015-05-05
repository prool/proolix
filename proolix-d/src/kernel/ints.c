#include <prool.h>

#include <conf.c>
#include "kernel.h"

void interrupt done(void)
{
puts("\n INTERRUPT  ");
}

void interrupt CtrlAltDel(void)
{
puts("\nU pressed Control Alt Del\n");
}

void interrupt Int1(void)
{
puts("\nInterrupt 1 - Trace\n");
}

void interrupt Int2(void)
{
puts("\nInterrupt 2 - Non-maskable  (NMI)\n");
}

void interrupt Int3(void)
{
/* puts("\nInterrupt 3 - Breakpoint\n"); */
}

void interrupt Int4(void)
{
puts("\nInterrupt 4 - Overflow trap\n");
}

void interrupt Int6(void)
{
puts("\nInterrupt 6 - Invalid opcode (186,286,386)\n");
}

void interrupt Int7(void)
{
puts("\nInterrupt 7 - Coprocessor not available (286,386)\n");
}

void interrupt Int75(void)
{
puts("\nInterrupt 75 - numeric coprocessor NMI error (AT,XT286,PS50+)\n");
}
