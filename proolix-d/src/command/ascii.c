#include <stdio.h>
#include <conio.h>

#include "prool.h"

void main (int argc)

{int i; char c;

for (i=0;i<256;i++)
  {
  c=i;
  switch (c)
    {
    /* case 7: */
    /* case 8: */
    /* case 9: */
    case 10:
    case 13: c='.';
    }
  if (argc==2) textattr(c);
  putch (c);
  putch (' ');
  if (i&&((i&15)==15)) {textattr (7); puts("");}
  }
textattr (7);
}
