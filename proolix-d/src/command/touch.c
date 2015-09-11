/*--------------------------------------------------------------------------*/
/*                                touch                                     */
/*--------------------------------------------------------------------------*/

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <dos.h>

#include "prool.h"

void help(void)
{
puts("Uses: touch [-c] [files...]");
}

int main(int argc, char *argv[] )
{
int i, j, h, n, Files=0, Flagc=0;
struct time t;
struct date d;
struct ftime ft;

argc--;

i=1;
do
  {
  if (argc)
    {
    if (argv[i][0]=='-')
      {
      n=(int)strlen(argv[i]);
      for (j=1;j<n;j++)
        switch (argv[i][j])
          {
          case 'c': Flagc=1; break;
          default : help(); return 1;
          }
      }
    else
      {
      Files++;
      if (Flagc)
        {
        if ((h=open(argv[i],O_RDONLY))==-1) continue;
        else close(h);
        }
      if ((h=open(argv[i],O_WRONLY))!=-1)
        {
        gettime(&t);
        getdate(&d);
        ft.ft_tsec  = t.ti_sec/2;
        ft.ft_min   = t.ti_min;
        ft.ft_hour  = t.ti_hour;
        ft.ft_day   = d.da_day;
        ft.ft_month = d.da_mon;
        ft.ft_year  = d.da_year;
        j=setftime(h,&ft);
        j+=close(h);
        }
      else j=-1;
      if (j) printf("touch: error on touching file %s",argv[i]);
      }
    }
  }
while (i++<argc);

if (!Files) {help(); return 1;}

return 0;
}
