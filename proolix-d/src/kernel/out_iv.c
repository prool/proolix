#include <prool.h>

#include <conf.c>

#include "kernel.h"

int OutIntVector (int i)
{
return printf("%2x %4x:%4x\n",i,FP_SEG(getvect(i)),FP_OFF(getvect(i)));
}
