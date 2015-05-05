#include <prool.h>

#include <conf.c>
#include "kernel.h"

#define Prec 2
#define Zero 0

/*----------------------------------------------------------------------------*/
/*                     E    M A I N                                           */
/*----------------------------------------------------------------------------*/
void e(int argc, char far*argv[])
{
int     i;

printf("argc = %i\nProg name = %s\n",argc,argv[0]);

argc += 1;

for (i=0; i<argc; i++)
{printf("Argument # %i  %s\n",i,argv[i]);}

puts("");

return;
}
