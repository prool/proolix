#include <stdio.h>

#include "prool.h"

main()
{int i,j;
for (;;)
  {
  for (i=0;i<80;i++)
    {
    for (j=0;j<i;j++) putch(' ');
    puts("-");
    for (j=0; j<32000; j++);
    if (kbhit()) {getch(); return;}
    }
  }
}
