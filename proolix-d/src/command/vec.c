#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <prool.h>

#define STRING_LIMIT 24

int string_counter;

void OutIntVector (int i)
{int far *ff;
ff=MK_FP(0,i<<2);
printf("%2x %4x:%4x\n",i,*(ff+1),*ff);
if (++string_counter>=STRING_LIMIT)
  {
  printf ("press any key");
  string_counter=0;
  if (getch()==ESC) {printf("\n");exit(1);}
  printf("\n");
  }
}

void main (int argc, char *argv[])
{int i;
string_counter=0;

if (argc==2)
  {
  OutIntVector(htoi(argv[1]));
  return;
  }
else
  for (i=0;i<256;i++) OutIntVector(i);
}