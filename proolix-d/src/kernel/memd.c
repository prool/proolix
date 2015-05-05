#include <prool.h>

#include <conf.c>

#include "kernel.h"

char cmd [MAX_LEN_STR];

void memd(void)
{
char far * Adr0;
char far * Adr;
char c;
int i;
char Option = 'H';
int segm, off;

/* Adr0 = (char far *) 0; */
segm = 0;
off = 0;
Adr0 = MK_FP (segm, off);

printf("End=%x:%x\nUse /? command for help\n",FP_SEG(end()),FP_OFF(end()));

while (1)
  {
  printf("%4x:%4x ",FP_SEG(Adr0),FP_OFF(Adr0));
  switch (Option)
    {
    case 'A':
    case 'M': for(i=0,Adr=Adr0;i<16;i++,Adr++)
                            {
                                if (*Adr>=' ') putch(*Adr);
                                else putch('.');
                                printf("  ");
                            }
                if (Option=='A')break; else puts("");
              printf("          ");
    case 'H': for(i=0,Adr=Adr0;i<16;i++,Adr++){printf("%2X ",*Adr);}
                          break;
                }
  puts("");
  putch(':');
  gets(cmd);

  i=0; while (cmd[i]) if (cmd[i]=='\r') {cmd[i]=0; break;} else i++;

  if (cmd[0]=='/')
    {
    switch (c=toupper(cmd[1]))
      {
      case 'R': saycsip(); break;
      case 'Q': puts(""); return;
      case 'A':
      case 'H':
      case 'M': Option=c; break;
      case 'S': segm=htoi(cmd+2); Adr0=MK_FP(segm, off); break;
      case 'O': off=htoi(cmd+2); Adr0=MK_FP(segm, off); break;
      case '?':puts(
"Commands:\n\
/Q - Quit\n\
/A - Ascii mode\n\
/H - Hex mode\n\
/M - Mix (hex & ascii) mode\n\
/S<hex_value> - set Segment\n\
/O<hex_value> - set Offset\n\
/R - registers dump\n\
Enter - next string"); break;
      default: puts("Invalid command");
      }
    printf("\n ");
    }
  else if (!cmd[0]) {off+=16; Adr0 = MK_FP(segm, off);}
  else ;
  }
}
