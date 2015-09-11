char    *suxx = (void *)0;
extern int must_die;

#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
#include <dir.h>
#include <limits.h>
#include <graphics.h>

#include <prool.h>

#define Prec 2
#define Zero 0

/*--------------------------------------------------------------------------*/
/*                     E    M A I N                                           */
/*--------------------------------------------------------------------------*/
void main (int argc, char *argv[])
{
int         i, j;
struct date d;
struct time t;
char curdir [PATH_MAX+1];
union REGS regs;

j=argc;

getdate(&d);
gettime(&t);

printf("Date is %2i-%02i-%02i %2i:%02i:%02i\n\n",
d.da_day,d.da_mon,d.da_year+1980,
t.ti_hour,t.ti_min,t.ti_sec);

printf("pwd=%Fs\n",getcwd(curdir,PATH_MAX+1));

regs.h.ah=15;
regs.x.flags=0;
int86(0x10,&regs,&regs);
printf("Current videomode = %i, page = %i\n",regs.h.al,regs.h.bh);

printf("argc = %i\n",j);
printf("Prog name = %s\n",argv[0]);

for (i=0; i<=j; i++)
  {
  printf("Argument # %i  ",i);
  if (argv[i]!=NULL) puts(argv[i]); else puts("(null)");
  }

printf("\nEnvironment:\n\n");
for (i=0; *(environ+i) != NULL; i++)
  {
  printf("environ.string=%Fs\n",*(environ+i));
  }
}
