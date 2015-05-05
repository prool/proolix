#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define MaxX 79
#define MaxY 24
#define MinX 1
#define MinY 1
#define Slowly 3000
/* #define Trace */
#define NX 100
#define NoStatic
/* #define BackGround */
/* #define ClrScr */
/* #define Fish */
#define Ice
/* #define Blink */

#define XONIX0 1
#define XONIX1 255

#define FRONTIER '~'

#ifndef XONIX1
#define XONIX1 XONIX0
#endif

int main(argc)
int argc;
{
int i, j;
int x[NX],y[NX];
int dx[NX],dy[NX];
int Color [NX];
char Xonix[NX];
#ifndef Trace
int oldx,oldy;
#endif
#ifdef Slowly
int ii;
#endif

clrscr();

if (argc==1)
  {
  gotoxy(20,10);
  printf("Press any key");

  for (i=0;!kbhit();i++)if(!i)putch('.');
  i+=getch();
  srand(i);
  puts("");
  }

#ifdef BackGround
for (i=0;i<24;i++)
  {
  for (j=0;j<79;j++)
    putch('Û');
  puts("");
  }
#endif

#ifdef ClrScr
clrscr();
#endif

#ifdef FRONTIER
if(MinY>0) for(i=1;i<=MaxX;i++) {gotoxy(i,MinY
#ifndef Ice
-1
#endif
);putch(FRONTIER);}
#endif

for (j=0;j<NX;j++)
  {
  x[j]=random(MaxX-MinX-1)+MinX+1;
  y[j]=random(MaxY-MinY-1)+MinY+1;
  dx[j]=1-random(3);
  dy[j]=1-random(3);
  Color[j]=random(16);
  #ifdef Fish
  dy[j]=0;
  #endif
  #ifdef NoStatic
  if (!dx[j] && !dy[j]) dx[j]=-1;
  #endif
  Xonix[j]=random(XONIX1-XONIX0+1)+XONIX0;
  if (Xonix[j]==7) Xonix[j]=XONIX0;
  else if (Xonix[j]==8) Xonix[j]=XONIX0;
  gotoxy(x[j],y[j]);
  putch(Xonix[j]);
  }

while(1)
{
if (kbhit()) break;
for (i=0;i<NX;i++)
  {
  textattr(i);
  if (kbhit())break;
  if (x[i]==MaxX) dx[i]=-dx[i]; else if (x[i]==MinX) dx[i]=-dx[i];
  if (y[i]==MaxY) dy[i]=-dy[i]; else if (y[i]==MinY) dy[i]=-dy[i];
  #ifndef Trace
  oldx=x[i];oldy=y[i];
  #endif
  x[i]+=dx[i]; y[i]+=dy[i];
  #ifndef Trace
  gotoxy(oldx,oldy);putch(' ');
  #endif
  gotoxy(x[i],y[i]);
  #ifdef Blink
  Xonix[i]^=1;
  #endif
  putch(Xonix[i]);
  #ifdef Slowly
  for (ii=1;ii<Slowly;ii++);
  #endif
  }
}
getch();
return 0;
}
