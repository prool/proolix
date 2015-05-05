/*----------------------------------------------------------------------------
                       T E R M I N A T O R
------------------------------------------------------------------------------
The terminal communication program for OS Proolix
------------------------------------------------------------------------------
Version 0.0.0 16-Oct-97
------------------------------------------------------------------------------*/

#include <stdio.h>

int main (void)
{
int port;
char str [80];

printf("terminator\n\n");

printf("Port no.? ");
gets(str);
scanf(str,"%i",&port);
printf("port=%i\n",i);

return 0;
}
