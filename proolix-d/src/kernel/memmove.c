#include <prool.h>

#include <conf.c>
#include "kernel.h"

/****************************************************************************/
void far *memmove (void far *dest, const void far *src, size_t n)
{char far *IBuf;
/* if (dest==NULL) return NULL; */
/* if (src==NULL) return NULL; */
if((IBuf=malloc(n))==NULL) {puts("memmove: No mem"); return NULL;}
memcpy(IBuf,src,n);
memcpy(dest,IBuf,n);
free(IBuf);
return dest;
}
/****************************************************************************/

