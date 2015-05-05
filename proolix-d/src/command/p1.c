#include <conio.h>
#include <stdio.h>
#include <process.h>

void saycsip(void);

void main (void)
{int i=0xAAAA,j=0xBBBB;
j=fork();
if (j==-1) puts("fork error");
else
  if (j)
    { /* parent */
    while (1)
      {
      putch('P');
      for (i=0;i<30000;i++) j++;
      /* L: goto L; */
      }
    }
  else
    { /* child */
    while (1)
      {
      putch('c');
      for (i=0;i<30000;i++) j++;
      /* L1: goto L1; */
      }
    }
}
