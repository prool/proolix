/* Mouse shell for Proolix (aka Desktop for Proolix) */

#include <conf.c>

#include "kernel.h"

#include <bios.h>
#include <stddef.h>

#include "msh.h"

void help(void);
void ladder(void);

/*---------------------------------------------------------------------------*/
void msh_pause(long int L)
{long int k;

for ( k=0 ; k < L ; k++);
}
/*---------------------------------------------------------------------------*/
void drebezg(void)
{
/*    while (kbhit())getch();*/
}
/*---------------------------------------------------------------------------*/
void msh_ident(void)
{
gotoxy(20,0);
printf("\007XProolix for BK-0010 ;) Ver. 0.0.0.6  4-Mar-94");
/* History
0.0.0.6  4-Mar-94 - сделал правильную работу с com-портами
0.0.0.5  2-Mar-94 - делаю правильную работу с com-портами по книге Фроловых
(см. коммент в bios.c)
0.0.0.4  1-Mar-94
0.0.0.3 27-Feb-94 - см. комментарий в kernel.c на данную дату
0.0.0.2 25-Feb-94
0.0.0.1 25-Feb-94 - эта версия не имела в себе номера версии, но была уже
анонсирована в kharkov.friends
*/
}
/*---------------------------------------------------------------------------*/
void atsay(int x, int y, char * str)
{
gotoxy(x,y);
printf(str);
}
/*---------------------------------------------------------------------------*/
void attr(void)
{unsigned int i,j;
for (i=0;i<16;i++)
  for (j=0;j<16;j++)
    {
    gotoxy(i,j);
    textattr(j+(i<<4));
    putch('*');
    }
}
/*---------------------------------------------------------------------------*/
void icon(struct DeskTop Desk [], int i)
{
gotoxy(Desk[i].x,Desk[i].y);
printf(Desk[i].Name);
}
/*---------------------------------------------------------------------------*/
int execm(void)
{
int i;
int k, b, MaxFolder;
long ii;
unsigned int j;
char far *rec;
int Chars;
int Files;
struct dirent far *D;

if (CurrentDevice==-1) {puts("No mount"); return -1;}

for(i=0;i<MAX_LINE;i++)
  {Folder[i].x=0;Folder[i].y=i;}

Files=0;
for(ii=RootBeg; ii<=RootEnd; ii++)
  {
  if((b=LoadCache(ii))==-1) {gotoxy(0,0); puts("Read error"); return -1;}
  rec = (*(Cache+b)).M;
  for (j=0;j<SECTOR_SIZE/32;j++,rec+=32)
    {
    Chars=0;
    D=(void far *)rec;
    if (!(*D).d_name[0] || ((*D).d_name[0]==0xe5))
      {
      /*if (Files<MAX_LINE)
        {
        strcpy(Folder[Files].Name,"<Unused>");
        }
      else
        goto br;
      */
      }
    else
      if (Files<MAX_LINE)
        {
        for (k=0;k<8;k++)
          if ((*D).d_name[k]!=' ')
            Folder[Files].Name[Chars++]=tolower((*D).d_name[k]);
        if ((*D).d_name[k] && ((*D).d_name[k]!=' '))
          Folder[Files].Name[Chars++]='.';
        for (k=8;k<11;k++)
          if ((*D).d_name[k]!=' ')
            Folder[Files].Name[Chars++]=tolower((*D).d_name[k]);
        Files++;
        }
      else
        goto br;
    }
  }
br:MaxFolder=Files;
while(1)
  {
  for (i=0;i<MaxFolder;i++) icon(Folder,i);
  textattr(Norm);
  for (i=0;i<MaxFolder;i++)
    {
    drebezg();
    textattr(Norm); if (!i) icon(Folder,MaxFolder-1); else icon(Folder,i-1);
    textattr(Inverse); icon(Folder,i); textattr(Norm);
    msh_pause(120000L);
    j=kbhit();
    gotoxy(70,1);printf("%4X",j);
    if (j)
      {
      getch();
      drebezg();
      gotoxy(0,0);
      if (!i) return 0;
      if (execve(Folder[i].Name,NULL,NULL)) {atsay(20,0," [EXEC ERROR] ");}
      drebezg();
      atsay(0,24," Press any key for continue ");
      getch();
      break;
      }
    }
  }
return -2;
}
/*---------------------------------------------------------------------------*/
void interrupt far msh(void)
{int i;
int j;
int _argc;
char far * _argv [MAX_ARG];

clrscr();

msh_ident();
#ifdef MOUSE
gotoxy(25,1);
printf("Com port initialization: %x\n",
inicom(PORT, Baud300 | None | StopBit1 | Bit8 ));
#endif
/*saycsip();*/

for (i=0;Desk[i].x!=-1;i++);
DeskNo=i;

_argc=1;
_argv[0]="cmd";
_argv[1]=NULL;

while(1)
  {
  textattr(Norm);
  for (i=0;i<DeskNo;i++) icon(Desk,i);
  drebezg();
  for (i=0;i<DeskNo;i++)
    {
    textattr(Norm); if (!i) icon(Desk,DeskNo-1); else icon(Desk,i-1);
    textattr(Inverse); icon(Desk,i); textattr(Norm);
    msh_pause(120000L);
    j=kbhit();
    gotoxy(70,1);printf("%4X",j);
    if ( j ) /*(j!=0x8061)&&(j!=0x8060)*/
      {
      getch();
      gotoxy(0,0);
      drebezg();
        switch(i)
        {
        case  0: break;
        case  1: execm(); break;
        case  2: attr(); break;
        case  3: textattr(Norm); clrscr(); break;
        case  4: mem(); break;
        case  5: _argc=2; _argv[1]="-l"; _argv[2]=NULL;
                 l(_argc,_argv); break;
        case  6: help(); break;
        case  7: _argc=2; _argv[1]="-l"; _argv[2]=NULL;
                 e(_argc,_argv); break;
        case  8: vec(_argc,_argv); break;
        case  9: clrscr(); ladder(); break;
        case 10: reboot();
        case 11: geninterrupt(0); break;
        case 12: geninterrupt(0x18); break;
        case 13: geninterrupt(0x19); break;
        case 14: printf("%X\n%X\n%X\n%X",com_address(0),com_address(1),
                 com_address(2),com_address(3));
        default:;
        }
      drebezg();
      break;
      }
    }
  }
}
