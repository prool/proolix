#include <prool.h>

#include <conf.c>
#include "kernel.h"

/****************************************************************************/
char far *strchr (const char far *str, int c)
{
/* if (str==NULL) return NULL; */
while (*str)
  if (*str==c)
    return (char far *) str;
  else str++;
return NULL;
}
/****************************************************************************/
