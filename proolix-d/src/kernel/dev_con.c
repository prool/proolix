#include <prool.h>

#include <conf.c>
#include "kernel.h"

int console_read (int minor)
{
return getch();
}

int console_write (int minor, int byte)
{
return putch (byte);
}

