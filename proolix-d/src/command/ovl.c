/* Kernel's overlay */

/* #define DEBUG */
#define ARGC 0 /* 0 1 */
#define FONT_LEN 4096
/* #define _DAYTIME_ */ /* daytime color scheme */

#include <prool.h>

#include <conf.c>
#include <limits.h>
#include <unistd.h>
#include "..\kernel\kernel.h"

#ifdef B_COOLWARE
unsigned int Speed( unsigned short CPUid );
long CPU_Type(void);

int Shift;
long CPUFix;

char *Processor [] = {
"8088",
"8086",
"V20",
"V30",
"188",
"186",
"286",
"386SX",
"386DX",
"386SL",
"486SX",
"486DX",
"Cyrix 486",
"486DLC",
"P5",
"Cyrix M1 (586)",
"IMHO, 686 (P6, Pentium Pro)"
};

char *CoProcessor [] = {
"unknown",
"unknown",
"none",
"Weitek 1167",
"8087",
"8087 + Weitek 1167",
"unknown",
"unknown",
"80287",
"80287 + Weitek 1167",
"Cyrix 2C87",
"Cyrix 2C87 + Weitek 1167",
"80387",
"80387 + Weitek 1167",
"Cyrix 3C87",
"Cyrix 3C87 + Weitek 1167",
"486 internal",
"486 internal + Weitek 1167",
"Cyrix 4C87",
"Cyrix 4C87 + Weitek 1167",
"Intel 80287XL",
"Intel 80287XL + Weitek 1167",
"IIT 2C87",
"IIT 2C87 + Weitek 1167",
"IIT 3C87",
"IIT 3C87 + Weitek 1167"
};
#endif
/****************************************************************************/
void main(
        #if ARGC
        int argc, char *argv[]
        #endif
        )
{int i;
char hour, min, secund, flag;
struct date d;
struct time t;
long l; int F;
union REGS inregs;
#ifndef CMOS
long Tick;
#endif
#if defined(_VGA_) && CODETABLE==_RUSSIAN_
int font_lines;
char *FontName;
int h;
#endif

#if ARGC
if (argc!=2) goto l;
if (argv[1][0]!='\r') {l: printf("This is overlay. Not executable\n"); exit(1);}
#endif

#ifdef _HERCULES_
  #ifdef _DAYTIME_
  textattr(10);
  #else
  textattr(7);
  #endif
#endif

puts("by Serge Pustovoitoff, http://prool.kharkov.org, prool@itl.net.ua\n"
#if 0
"Proolix comes with ABSOLUTELY NO WARRANTY.\n\
This is free software\n\n\
WWW http://prool.kharkov.org/proolix/\n\
Voice phone +380(572)206-574\n"
#endif
"");

#if 1
/* Numlock off */
asm mov ax,40h;
asm mov es,ax; /* ES:=40h */
asm and byte ptr es:17h,0DFh;
#endif

printf("Scroll Lock - russian keyboard mode\n\
Alt-F1 -- Alt-F8 - switch videopages\n");

#ifdef CMOS
/* puts("CMOS support enabled"); */
if (readRTC(&hour,&min,&secund,&flag))
 {
 puts("FATAL ERROR: *** No CMOS !!! ***");
 }
else
 {
 #if 0
   #if 0
   printf("CMOS time ");
   printf("%2X:%2X:%2X flag= %i\n",hour,min,secund,flag);
   #endif
 char *Primary [4] =
 {"none or EGA/VGA","40 column color","80 column color","monochrome"};
 outportb(0x70,0x14);
 i=(inportb(0x71) & 0x30)>>4;
 printf("CMOS: Primary display set %s\n",Primary[i]);
 #endif
 }
#else
ReadTick(&Tick,&flag);

printf("Ticks %li flag=\n",Tick,flag);
#endif

#ifdef CLRSCR
clrscr();
#endif

getdate(&d);
gettime(&t);

printf("%2i-%02i-%02i %2i:%02i:%02i\n",d.da_day,d.da_mon,d.da_year+1980,
t.ti_hour,t.ti_min,t.ti_sec);

#ifdef B_COOLWARE
F=(int)((((CPUFix*Shift)/Speed(6))+5L)/10L);
l=CPU_Type();
printf("CPU: %s, %i MHz, ",Processor[(int)(l&0xFF)],F);
printf((l&0xFF00)?"V86 mode":"real mode");
printf(", coprocessor: %s\n",CoProcessor[(int)((l&0xFF0000L)>>16)]);
#endif

#if defined(_HERCULES_) && defined(_VIDEO_GRAPH_)
/* set Hercules to graph mode */
inregs.x.ax=8;
inregs.x.flags=0;
int86 (0x10, &inregs, &inregs);
#endif

/* set cursor */
inregs.x.ax=0x0100;
inregs.x.cx=0x0C0D;
inregs.x.flags=0;
int86 (0x10, &inregs, &inregs);

/* load russian font */ /* Portions Copyright (C) by G.Shepelev */
/* определить размер литерной матрицы */

#if defined(_VGA_) && CODETABLE==_RUSSIAN_
#if 0 /* font load worked witn bug!!! :((( */

font_lines=*(int far *)MK_FP(0x40,0x85);
/* printf("Font size %i lines\n",i); */

switch (font_lines)
  {
  case  8: FontName="08.fnt"; break;
  case 14: FontName="14.fnt"; break;
  case 16: FontName="16.fnt"; break;
  default: printf("Invalid font size=%i lines\n",font_lines);
  }

chdir(FONT_DIR);

h=open(FontName,O_RDONLY);

if (h!=-1)
  {char font_buf[FONT_LEN];
  read(h,font_buf,FONT_LEN);
  close(h);
  _ES=_SS;
  _BH=font_lines;       /* bh - число строк в матрице                       */
  _BL=0;                /* bl - номер банка знакогенератора                 */
  _AX=0x1100;           /* функция 0 - загрузка пользовательского шрифта    */
  _CX=256;              /* число символов в таблице                         */
  _DX=0;                /* номер первого определяемого символа              */
  asm   push    bp;     /*                                                  */
  _BP=(int)font_buf;    /* ES:BP - адрес таблицы                            */
  asm int 10h;
  asm   pop     bp;
  }
else
  printf("Can't open font file %s\n",FontName);
#endif /* #if 0 */

#endif /* defined(_VGA_) && CODETABLE==_RUSSIAN_ */

chdir(HOME_DIR);

}
