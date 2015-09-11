#include <prool.h>

#include <conf.c>
#include "kernel.h"

void qsort (void far *base, size_t nmemb, size_t size,
        int (far *compar)(void far *, void far *))
{int i, Permute; void far *b1, far *b2; char far *Bolvan;
if (base==NULL) return;
if ((Bolvan=malloc(size))==NULL) return;
do
  {
  Permute=0;
  for (i=0; i<(nmemb-1); i++)
    {
    if (compar(b1=(char far *)base+(int)( i   *size),
               b2=(char far *)base+(int)((i+1)*size) )>0)
      { /* swap */
      Permute=1;
      memcmp(Bolvan,b1,size);
      memcmp(b1,b2,size);
      memcmp(b2,Bolvan,size);
      }
    }
  }
while (Permute);
free(Bolvan);
}
