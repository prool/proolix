#include <conio.h>

int main (void)
{int i,j;
j=0;
while (1)
  {
  putch('3');
  /* asm int 85H; */
  for (i=0;i<30000;i++) j++;
  }
return j;
}
