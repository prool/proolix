#define VER "Proolix MSDOS emulator v. 0.0.23 14-Feb-99"

/* History (in reverse order)

v. 0.0.23 14-Feb-99
v. 0.0.22  7-Feb-99
v. 0.0.21 25-Aug-97
v. 0.0.20 17-Aug-97
v. 0.0.19 15-Aug-97
v. 0.0.18 30-Jun-97
v. 0.0.17 14-May-97
v. 0.0.16W 14-May-97
v. 0.0.15 11-May-97
v. 0.0.14  2-May-97
v. 0.0.14  1-May-97
v. 0.0.13 30-Apr-97
v. 0.0.12 29-Apr-97
v. 0.0.11 28-Apr-97
v. 0.0.10 27-Apr-97
v. 0.0.9   9-Apr-97
v. 0.0.8   7-Apr-97
v. 0.0.7   7-Apr-97
v. 0.0.6   2-Apr-97
v. 0.0.5  22-Mar-97
v. 0.0.5  21-Mar-97
v. 0.0.5  20-Mar-97
v. 0.0.4  19-Mar-97
v. 0.0.3  18-Mar-97
v. 0.0.2  17-Mar-97
v. 0.0.1   9-Mar-97
v. 0.0.0   2-Mar-97 */

/* #define DEBUG */
#define HEADER_SIZE 28
/****************************************************************************/
#include "mse.h"
#include <conf.c>
#include <prool.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <process.h>
#include <string.h>
#include <fcntl.h>
#include <alloc.h>
#include <dos.h>
#include <struct.h>
#include <dir.h>
#include <psp.h>
#include <sys/stat.h>
/****************************************************************************/
char Sec1 [SECTOR_SIZE];
char huge *aPSP=0;
jmp_buf return_point;
char string [MAX_COMMAND_STRING];
char osmajor=DOS_MAJOR, osminor=DOS_MINOR, osbreak;
struct DTAfind far * DTAaddr;
struct country_structure Ukraine;
char far * pEnv, far * curEnv;
int TSR=0;
char wd [PATH_MAX+1];
/****************************************************************************/
char allow_dos_functions [255];
char verbose_dos_functions [255];
char allow_change_int [255];
int msdos_verbose=0;
int msdos_pause=0;
char OldInt [INTERRUPT_TABLE_SIZE];
/****************************************************************************/
void hexdump(char *header, char huge *adr, int ctr)
{int i;
printf(header);
for (i=0;i<ctr;i++)printf("%02X",adr++);
}
/****************************************************************************/
void SaveVectors(void)
{
__cli__();
memcpy(OldInt,MK_FP(0,0),INTERRUPT_TABLE_SIZE);
__sti__();
}
/****************************************************************************/
void RestoreVectors(void)
{
__cli__();
memcpy(MK_FP(0,0),OldInt,INTERRUPT_TABLE_SIZE);
__sti__();
}
/****************************************************************************/
void far case_map(void)
{
printf("===CASE===");
}
/****************************************************************************/
void initialization (void)
{int i;
char *Env1;

SaveVectors();

asm     extrn   _Int21:far;
asm     extrn   _Int27:far;
asm     extrn   _Int29:far;

asm     mov     ax,offset _Int20;
asm     mov     i,ax;

setvect(0x20,(void interrupt(*)(void))MK_FP(_CS,i));

asm     mov     ax,offset _Int27;
asm     mov     i,ax;

setvect(0x27,(void interrupt(*)(void))MK_FP(_CS,i));

asm     mov     ax,offset _Int21;
asm     mov     i,ax;

setvect(0x21,(void interrupt(*)(void))MK_FP(_CS,i));

asm     mov     ax,offset _Int29;
asm     mov     i,ax;

setvect(0x29,(void interrupt(*)(void))MK_FP(_CS,i));

for (i=0;i<5;i++)
  {struct stat st;
  if (!fstat(i,&st))
    {int h;
    if ((h=open("/",O_RDONLY))!=i)
      {
      if (h==5) { close(h); break; }
      }
    }
  }

for (i=0;i<=255;i++)
  {
  allow_dos_functions [i]=1;
  allow_change_int [i]=1;
  verbose_dos_functions [i]=1;
  }

verbose_dos_functions[1]=0;
verbose_dos_functions[2]=0;
verbose_dos_functions[6]=0;
verbose_dos_functions[7]=0;
verbose_dos_functions[8]=0;
verbose_dos_functions[9]=0;
verbose_dos_functions[0xB]=0;
verbose_dos_functions[0x40]=0;

#if 0
allow_dos_functions [0]=1;
allow_dos_functions [1]=1;
allow_dos_functions [2]=1;
allow_dos_functions [7]=1;
allow_dos_functions [8]=1;
allow_dos_functions [9]=1;
allow_dos_functions [0xB]=1;
allow_dos_functions [0x25]=0;
allow_dos_functions [0x30]=1;
allow_dos_functions [0x31]=1;
allow_dos_functions [0x35]=1;
allow_dos_functions [0x40]=1;
allow_dos_functions [0x4A]=1;
allow_dos_functions [0x4C]=1;
#endif

Ukraine.date_time=0; /* 0 = month day year,  hh:mm:ss  (USA)
                        1 = day month year,  hh:mm:ss  (Europe)
                        2 = year month day,  hh:mm:ss  (Japan) */
memcpy(Ukraine.currency,"karb",5);
memcpy(Ukraine.thousands_sep,",",2);
memcpy(Ukraine.decimal_sep,".",2);
memcpy(Ukraine.date_sep,"-",2);
memcpy(Ukraine.time_sep,":",2);
Ukraine.currency_lead = 0; /* 0 = symbol leads, without space
                              1 = symbol follows, without space
                              2 = symbol leads, one space
                              3 = symbol follows, one space
                              4 = symbol replace decimal separator */
Ukraine.digital_after_dec = 2;
Ukraine.time_format = 0; /* Bit 0   = 0  12 hour clock
                                    = 1  24 hour clock */
Ukraine.case_map_call = (long) MK_FP(_CS,case_map);
Ukraine.asciiz_sep='/';

pEnv = malloc(ENV_SIZE);
#ifdef DEBUG
printf("pEnv=%04X:\n",FP_SEG(pEnv));
#endif

Env1="COMSPEC=C:\DSH.COM";
strcpy(pEnv,Env1);
curEnv=pEnv+(int)strlen(Env1);
*curEnv++=0;
*curEnv++=0;
*curEnv++=1;

}
/****************************************************************************/
void deinitialization (void)
{
RestoreVectors();
free(pEnv);
}
/****************************************************************************/
void help (void)
{
printf("\n\nMSDOS emulator internal command:\n\n\
help - this help\n\
ver - version\n\
setver - set DOS version\n\
cd - print workdir\n\
cd <dir> - change dir\n\
setpause [0|1] - set MSDOS debuggin' pause\n\
verbose [0|1] - set MSDOS debuggin' verbose mode\n\
");
}
/****************************************************************************/
void far cpm (void)
{
printf("\nCalled CP/M entry!\n");
}
/****************************************************************************/
int ViewEXE (struct exe_header far *Buf)
{int c;
#ifndef _HERCULES_
textattr(EMULATOR_COLOR);
#endif
printf("Bytes on last page %04X\n",Buf->PartPag );
printf("Pages in file %04X\n",Buf->PageCnt );
printf("Relocations %04X\n",Buf->ReloCnt );
printf("Paragraphe in header %04X\n",Buf->HdrSize );
printf("MinMem %04X par\n",Buf->MinMem  );
printf("MaxMem %04X par\n",Buf->MaxMem  );
printf("SS:SP %04X:%04X\n",Buf->ReloSS,Buf->ExeSP);
printf("CheckSum %04X\n",Buf->ChkSum  );
printf("CS:IP %04X:%04X\n",Buf->ReloCS,Buf->ExeIP);
printf("Relocation table addr %04X\n",Buf->TablOff );
printf("Overlay No. %1X\n",Buf->Overlay);
  #if 0
  printf("\n1.Press ESC for quit, other key for continue\n");
  c=getch();
  #ifndef _HERCULES_
  textattr(DOS_COLOR);
  #endif
  if (c==ESC) return 1;
  #endif
return 0;
}
/****************************************************************************/
int execdos (char *argv0, char *params)
{
char ExeHeader [HEADER_SIZE];
int i, j, h, argc, nenv, restos, segment, offset_s;
long int FileLen, l, sec;
unsigned int len;
size_t SizeMemoryBlock;
char huge *mem, far *ptr, far *adr, far *EnvAdr;
struct exe_header far * H;
unsigned int RelocTable [2], BEGIN_SEG, I_OFF, I_SEG, RELO_SEG;
struct sPSP far *PSP;
char FlagCOM;
unsigned int MemTop;
jmp_buf Pointer;
long ExeLen, ReadLen;

if ((h=open(argv0,O_RDONLY))==-1) return 1;

FileLen=filelength(h);

/*** Чтение первого сектора и проверка на сигнатуру MZ ***/
restos = (SECTOR_SIZE>FileLen) ? (int)FileLen : SECTOR_SIZE;

if (read(h,Sec1,restos) != restos) goto R3;

printf("restos=%i\n",restos);
hexdump("Sec1=",Sec1,16);

if (!memcmp(Sec1,"MZ",2))
  { /* EXE */
  FlagCOM=0;
  memcpy((char far *)ExeHeader,(char far *)Sec1,HEADER_SIZE);
  H=(void far *)ExeHeader;
  printf(".EXE file. File length=%li\n",FileLen);
  if (ViewEXE(H))
    {
    close(h);
    return 1;
    }
  ExeLen=(H->PageCnt*512)-(16*H->HdrSize)-(H->PartPag);
  printf("Image length=%li bytes\n",ExeLen);
  SizeMemoryBlock=-1L; /*(H->PageCnt*(512/16)-H->HdrSize+H->MinMem)<<4-(H->PartPag)*/
  }
else
  { /* COM */
  FlagCOM=1;
  SizeMemoryBlock=((ComMemory -1U)<<4)+1;
  }

#if 0
printf("2.Press ESC for quit or other key for continue\n");
if (getch()==27) return 1;
#endif

#ifdef DEBUG
printf("SizeMemoryBlock=%08lX\n",SizeMemoryBlock);
#endif

if ( (void far *)(aPSP=malloc(SizeMemoryBlock)) == NULL )
  {
  printf("execdos: no memory. (Required %04X bytes)\n",SizeMemoryBlock);
  return 2;
  }

#ifdef DEBUG
printf("aPSP=%04X:%04X\n",FP_SEG(aPSP),FP_OFF(aPSP));
#endif

mem=aPSP+0x100;

#ifdef DEBUG
printf("mem=%04X:%04X\n",FP_SEG(mem),FP_OFF(mem));
#endif

if (FlagCOM==0) BEGIN_SEG=FP_SEG(aPSP)+0x10;

PSP=(void far *)aPSP;
PSP->CD20=0x20CDU;        /* int 20 h */

asm     push    CS;
asm     pop     ax;
asm     mov     i,ax;
asm     mov     ax,offset Int20;
asm     mov     j,ax;
PSP->Int22=(((unsigned long)i << 16) | j);

asm     push    CS;
asm     pop     ax;
asm     mov     i,ax;
asm     mov     ax,offset Int23;
asm     mov     j,ax;
PSP->Int23=(((unsigned long)i << 16) | j);

asm     push    CS;
asm     pop     ax;
asm     mov     i,ax;
asm     mov     ax,offset Int24;
asm     mov     j,ax;
PSP->Int24=(((unsigned long)i << 16) | j);

MemTop = (*((unsigned int far *)MK_FP(0,0x413)))<<6;
PSP->TopMemPar=MemTop;
PSP->op=0x9A;
PSP->Avail=0xFEF0;
PSP->cpm_segment=FP_SEG(cpm);
PSP->ParentSeg=0x50;
PSP->aEnv=FP_SEG(pEnv);
PSP->prevPSP=0xFFFFFFFFUL;
PSP->SS_SP=(long)MK_FP(FP_SEG(aPSP),_SP);
PSP->pHandle=(long)MK_FP(FP_SEG(aPSP),0x18);
PSP->sHandle=20;
PSP->FnCB[0]=0xCD;
PSP->FnCB[1]=0x21;
PSP->FnCB[2]=0xCB;
for (i=0;i<20;i++) PSP->aHandle[i]=0xFF;
DTAaddr=(void far *)((char far *)aPSP+0x80);

/* command string to PSP */
if (params==NULL)
  { /* комстрока пустая (только имя_программы) */
  /* l_bug: */
  l=1;
  strcpy((char far *)aPSP+0x81,"\r");
  }
else
  {
  l=(int)strlen(params);
  if (l>127) l=127;
  strcpy((char far *)aPSP+0x81,params);
  }
*(unsigned char far *)(aPSP+0x80)=l;
*(unsigned char far *)(aPSP+0x81+l)='\r';

if (FlagCOM)
  {
  memcpy((void far *)mem,Sec1,restos);
  mem+=restos;
  }
else
  { /* EXE */
  if(lseek(h,(H->HdrSize)<<4,SEEK_SET)==-1L)
    {R3:close(h); free((void far *)aPSP); return 3; }
  }

/* Чтение остальных секторов */
if (FlagCOM)
  { /* COM */
  ReadLen=FileLen-restos;
  }
else
  { /* EXE */
  ReadLen=ExeLen;
  }

sec=ReadLen/SECTOR_SIZE;
for (l=1;l<sec;l++)
  {
  if (read(h,(void far *)mem,SECTOR_SIZE)!=SECTOR_SIZE) break;
  mem+=SECTOR_SIZE;
  }

restos=(int)(ReadLen % SECTOR_SIZE);
read(h,(void far *)mem,restos);

if (FlagCOM==0)
  {/* EXE: Hастройка адресов */
  if(lseek(h,H->TablOff,SEEK_SET)==-1L) goto R3;
  for (i=0; i<H->ReloCnt; i++)
    {
    if(read(h,RelocTable,4)!=4) goto R3;
    I_OFF=RelocTable[0];
    I_SEG=RelocTable[1];
    RELO_SEG=BEGIN_SEG+I_SEG;
    *(unsigned int far *)MK_FP(RELO_SEG,I_OFF) =
      *(unsigned int far *)MK_FP(RELO_SEG,I_OFF)+BEGIN_SEG;
    }
  }

if (close (h)) {free((void far *)aPSP); return 3;}

len=2;

strcpy(curEnv,argv0);

/* EXEC ! */

if (FlagCOM) printf("EXEC!\n");

#if 1
{char far *mmm;
printf("Start point %04X:%04X ",H->ReloCS+BEGIN_SEG,H->ExeIP);
mmm=MK_FP(H->ReloCS+BEGIN_SEG,H->ExeIP);
for (i=0;i<16;i++) printf("%02X ",*mmm++);
printf("\n");
}
#endif

Pointer[0].j_flag=0x7202;
if (FlagCOM)
  { /* COM */
  Pointer[0].j_sp=0xFFFEU-len;
  Pointer[0].j_ss=FP_SEG(aPSP);
  Pointer[0].j_cs=FP_SEG(aPSP);
  Pointer[0].j_ip=0x100;
  Pointer[0].j_es=FP_SEG(aPSP);
  Pointer[0].j_ds=FP_SEG(aPSP);
  }
else
  { /* EXE */
  Pointer[0].j_sp=H->ExeSP;
  Pointer[0].j_ss=H->ReloSS+BEGIN_SEG;
  Pointer[0].j_cs=H->ReloCS+BEGIN_SEG;
  Pointer[0].j_ip=H->ExeIP;
  Pointer[0].j_es=FP_SEG(aPSP);
  Pointer[0].j_ds=FP_SEG(aPSP);
  }

longjmp (Pointer, 0);

return 0;
}
/****************************************************************************/
void main (void)
{
char argv0 [ARGV0LEN];
int i;

initialization();

printf(VER);
printf("\n\nUse `help` for help\n");

setjmp(return_point);

printf("\n");

while (1)
  {char *cc;
  printf("C>");
  gets(string);
  if (!string[0]) continue;
  for (i=0;i<ARGV0LEN;argv0[i++]=0);
  if ((cc=(char *)strchr(string,' '))==NULL)
    strncpy(argv0,string,ARGV0LEN);
  else
    {int l;
    if ((l=cc-string)>ARGV0LEN) l=ARGV0LEN;
    memcpy(argv0,string,l);
    }
  /* printf("argv0=`%s'\n",argv0); */
  if (!strcmp(argv0,"ver"))
    {
    printf("%s\nsetver=%i.%i\n",VER,osmajor,osminor);
    printf("msdos_pause=%i\n",msdos_pause);
    printf("msdos_verbose=%i\n",msdos_verbose);
    }
  else if (!strcmp(argv0,"help")) help();
  else if (!strcmp(argv0,"exit")) goto l_exit;
  else if (!strcmp(argv0,"cd"))
    {
    if (cc==NULL)
      {
      if (getcwd(wd,PATH_MAX+1)==NULL)
        puts("pwd error");
      else
        puts(wd);
      }
    else if (chdir(cc+1)) printf("Can't chdir %s\n",cc+1);
    }
  else if (!strcmp(argv0,"setver"))
    {
    osmajor=atoi(cc+1);
    cc=(char *)strchr(cc,'.');
    if (cc!=NULL) osminor=atoi(cc+1);
    printf("New version %i.%i\n",osmajor,osminor);
    }
  else if (!strcmp(argv0,"setpause"))
    {
    if ((*(cc+1))=='0') msdos_pause=0;
    else msdos_pause=1;
    printf("msdos_pause=%i\n",msdos_pause);
    }
  else if (!strcmp(argv0,"verbose"))
    {
    if ((*(cc+1))=='0') msdos_verbose=0;
    else msdos_verbose=1;
    printf("msdos_verbose=%i\n",msdos_verbose);
    }
  else
    {
    printf("\nmse: ");
    switch (execdos(argv0, cc))
      {
      case 0:  /* printf("OK"); */ break;
      case 1:  printf("not found\n"); break;
      case 2:  printf("no memory\n"); break;
      case 3:  printf("I/O error\n"); break;
      default: printf("invalid error\n"); break;
      }
    }
  }

/* printf("\n"); */
l_exit:
deinitialization();
}
