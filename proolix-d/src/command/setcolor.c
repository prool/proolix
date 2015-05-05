#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

void main(int argc, char *argv[])
{
if (argc==1)
  {
  printf("Current videoattr = %02X\n", getcolor());
  }
else
  {
  textattr(htoi(argv[1]));
  }
}
