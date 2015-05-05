#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

void main()
{
int i;

printf("View Key Codes\n\nPress Ctrl-D for exit\n\n");
while(1)
  {
  i=getch();
  printf("%i\n",i);
  switch(i)
    {
    case 4: printf("\n"); exit(0);
    /* case 0: i=getch(); printf("%i\n",i); */
    }
  }
}
