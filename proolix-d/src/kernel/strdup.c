#include <prool.h>

#include <conf.c>
#include "kernel.h"

/****************************************************************************/
char far *strdup(const char far *s)
{char far *new;
/* if (s==NULL) return NULL; */
if ( (new=malloc(strlen(s))) != NULL ) strcpy(new,s);
return new;
}
/****************************************************************************/

