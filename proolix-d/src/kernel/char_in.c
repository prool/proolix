#include <stdarg.h>
#include <kbd.h>
#include <bios.h>

#include <prool.h>

#include <conf.c>
#include "kernel.h"

#define Norm 2
#define Inverse 0x70
/*==========================================================================*/
/* #define DEBUG */
/*==========================================================================*/
#ifdef MOUSE
/*--------------------------------------------------------------------------*/
int getch(void)
{int j;
while ((j=getcom(PORT))==-2); /* Wait a click or move mouse
                                 (wait a byte from com port */
while (getcom(PORT)!=2); /* Это против дребезга мыши */
return j;
}
/*--------------------------------------------------------------------------*/
int kbhit(void)
{int j;
j=getcomstat(PORT);
if (j==-1) return 0; else return (j & 1);
}
/*--------------------------------------------------------------------------*/
#else /* no MOUSE */
/*--------------------------------------------------------------------------*/
char extended_kbd_code=0;
/*--------------------------------------------------------------------------*/
int getch(void)
{int c;

if (extended_kbd_code) {c=extended_kbd_code; extended_kbd_code=0; }
else
  {
  c=getch0();
  if ((char)c==0)
    {
    extended_kbd_code=c>>8;
    c=0;
    if ((extended_kbd_code>='h')&&(extended_kbd_code<='o'))
      {/* Alt-F1 .. Alt-F8 */
      new_video_page(extended_kbd_code-'h');
      extended_kbd_code=0;
      return getch();
      }
    }
  else
    c=(char)c;
  }

#if CODETABLE==_RUSSIAN_
if (*KbdStatus & SCROLL_LOCK) c=Russian[c];
#endif

return c;
}
/*--------------------------------------------------------------------------*/
int kbhit(void)
{
if (extended_kbd_code) return 1;
else return kbhit0();
}
#endif
/*--------------------------------------------------------------------------*/

/****************************************************************************/
int getchar(void)
{int c;
/* Судя по исходниками GNU file утилит (функция yesno()) при нажатии
клавиши Enter должен возвращаться код '\n', а не '\r' как в MSDOS.
А также getchar должна быть с эхом. */
c=getch();
#ifdef DEBUG
putch('[');
putch(c);
putch(']');
#endif
if (c!='\b') c=putchar (c=='\r'?'\n':c); /* '\b' - BREAK */
return c;
}
/****************************************************************************/
int getche (void)
{int c;
c=getch();
if (c<' ')
  if (c=='\b')
    return putch(c);
  else
    return c;
else return putch(c);
}
