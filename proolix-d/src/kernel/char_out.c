#include <stdarg.h>

#include <prool.h>

#include <conf.c>
#include "kernel.h"

#define NormalColor 14
#define Inverse 0x70
/*==========================================================================*/
/* #define DEBUG */
/*==========================================================================*/
int sema_putch=0;
/*==========================================================================*/
#if 1
int mputch(int c)
{
while (sema_lock(&sema_putch)==1);
PutchBIOS(c);
sema_unlock(&sema_putch);
return c;
}
#endif
/****************************************************************************/
int Puts1 (const char far *str)
{int i;
char c;
while((c=*str++)!=0)
  if ((i=mputch(c))==EOF) return EOF;
return i;
}
/****************************************************************************/
int putch(int c)
{
switch (c)
  {
  case '\n':
           mputch('\r');
           mputch('\n');
           #if 1
           if (More)
             if (++NLine>MAX_LINE)
               {
               NLine=1;
               Puts1("more");
               if (More==2) Puts1(". Press ESC for quit, or anykey for continue");
               if ((c=getch())==ESC) {errno=EFAULT; return EOF; }
               else {
                    errno=0;
                    mputch('\r');
                    mputch('\n');
                    return '\n';
                    }
               }
             else ;
           else ;
           #endif
           break;
  case 9:  /* TAB */
           mputch(' ');
           break;
  default:
           return mputch(c);
  }
return c;
}
/****************************************************************************/
int putchar (int c)
{
return putch(c);
}
/****************************************************************************/
int Puts0 (const char far *str)
{int i;
char c;
while((c=*str++)!=0)
  if ((i=putch(c))==EOF) return EOF;
return i;
}
/****************************************************************************/
int puts (const char far *str)
{
if (str==NULL) return puts("(null)");
if(Puts0(str)==EOF)return EOF;
return Puts0("\n");
}
