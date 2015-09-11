#include <prool.h>

#include <conf.c>
#include "kernel.h"

/****************************************************************************/
size_t strlen (const char far *s)
{
size_t i;
/* if (s==NULL) return NULL; */
i=0;
while (*s++)i++;
return i;
}
/****************************************************************************/

