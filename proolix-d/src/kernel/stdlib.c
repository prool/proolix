#include <prool.h>

#include <conf.c>
#include "kernel.h"

/****************************************************************************/
/* Целочисленная арифметика */
/****************************************************************************/
div_t div(int number, int denom)
{div_t R;
R.quot=number / denom;
R.rem=number % denom;
return R;
}
/****************************************************************************/
ldiv_t ldiv(long number, long denom)
{ldiv_t R;
R.quot=number / denom;
R.rem=number % denom;
return R;
}
/****************************************************************************/
int system (const char far *command)
{
if (command==NULL) return 0; else return 1;
}
/****************************************************************************/
char far *getenv (const char far *name)
{int i, lname;
lname=(int)strlen(name);
for (i=0;i<MAX_ENV;i++)
  if (memcmp(hEnv,name,lname)==0)
    if ((hEnv+i)[lname]=='=')
      {
      return hEnv+i+lname+1;
      }
return NULL;
}
