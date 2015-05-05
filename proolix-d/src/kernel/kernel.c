/*      OS Proolix Kernel

by 1993-2006 Serge Pustovoitoff, prool@itl.net.ua
http://prool.kharkov.org/proolix/
Free with sources
 ****************************************************************************/
#define VER "Ver. Feb 2006"
/****************************************************************************/

/* #define KERNEL_DEBUG */
/* #define NODISK */

#include <prool.h>

#include <conf.c>
#include <limits.h>
#include <unistd.h>

#include <struct.h>
#include <signal.h>
#include <setjmp.h>

#include <fcntl.h>
#include <errno.h>

#include <alloc.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <dos.h>
#include <dir.h>
#include <sys/types.h>
#include <conio.h>
#include <process.h>
#include <dirent.h>
#include <proto.h>
#include <psp.h>

#include "kernel.h"

/******************************** External variables ***********************/
#ifdef MOUSE
extern unsigned int PORT;
#endif
/*
Mode:
0 - если работает код ядра, или системный вызов из юзерского процесса,
1 - если работает код юзерского процесса.
User:
0 - если работает код ядра,
1 - если работает юзерский процесс или системный вызов из юзерского процесса
Multitask:
0 - однозадачный режим
1 - многозадачный режим
*/
int User, Multitask;
int HeadCnt, TrkSecs;
int SectorsOnCyl;
unsigned long FatSize;
int FatCnt=2;
unsigned long MaxClusters;
unsigned long MaxSectors;
/* extern unsigned int _Begin; */
extern int Trace;
unsigned int MemTop;
void far * Fin;
int emulation_mode; /* 0 - boot from disk/diskette; */
                    /* 1 - boot from file under MSDOS-session Windows 95 */

/******************************** Public variables *************************/

/* char NearBuffer [512]; */

int TextAttr=MAIN_TEXT_ATTR;

int _argc;
char far *_argv [MAX_ARG];
char far * far *pEnv;
char far *hEnv;
unsigned int Global;
unsigned int Counter;
char far *KbdStatus;
int curX=1, curY=1, maxX=80, maxY=25;
char far *curAddr;
char VideoAttrib=7;

/* long int Secund=0; */
#ifdef CMOS
unsigned char hour, min, secund;
#else
unsigned long Tick;
#endif
unsigned char century, year, month, day, flag;

#if CODETABLE==_RUSSIAN_
char Russian [256];
#endif

void interrupt (*OldTimerVec) (void);

unsigned long RootBeg, RootEnd, ResSecs;
unsigned long RootBegClu=0;
unsigned int CluSizeBytes;

struct BufPool far *Cache;

int CurrentDevice=-1;
int ReadOnly;
int NextDOSDrive='C';

        /* Глобальные переменные, доступные из юзерских процессов */
int errno=0;
int More=0;
int NLine=1;
        /* */

unsigned int ProcessPar;

unsigned int CluSize;
unsigned long DataStart;

int x, y;

struct MemBlk *MB;
int FATMode;
FILE FCBS [OPEN_MAX];
char MasterBootRecord [SECTOR_SIZE];

char far *OldInt;

FILE far *stdin ;
FILE far *stdout;
FILE far *stderr;

/* таблица специальных файлов (файлов типа Device) */

struct DevRecord
Dev [] =
{
/* name, major, minor, byte (1 - byte-oriented device, 0 - block-oriented) */
/* Один и тот же major номер устройства для байториентированных и для
   блокориентированных обозначает РАЗHЫЕ устройства. Это сделано для
   экономии места в таблицах */
  {"null",    0,0,1},
  {"console", 1,0,1},
  {"fd0",     0,0,0},
  {"fd1",     0,1,0},
  {"hd0",     1,0,0},
  {"hd1",     1,1,0},
  {"ttyS00",  2,0,1}, /* COM */
  {"ttyS01",  2,1,1},
  {"lp00",    3,0,1}, /* LPT */
  {"lp01",    3,1,1},
  {"mem",     4,0,1},
  {"kmem",    5,0,1},
  {"",-1,-1,-1}
};

/* таблица специальных файлов байториентированных */

struct DevByte TableByte [5] =
{
 {nulldev,nulldev,nulldev,nulldev,nulldev},
 {nulldev,nulldev,nulldev,nulldev,nulldev},
 {nulldev,nulldev,nulldev,nulldev,nulldev},
 {nulldev,nulldev,nulldev,nulldev,nulldev},
 {nulldev,nulldev,nulldev,nulldev,nulldev},
};

/* таблица специальных файлов блокориентированных */

struct DevBlock TableBlock [2] =
  { /* open, close,  strategy */
    {nulldev,nulldev,FDDstrategy}, /* FDD */
    {nulldev,nulldev,HDDstrategy}, /* HDD */
  };

unsigned long CurDirClu;

struct DeviceStruct Devices [LASTDRIVE];
  /*
        Devices [0] - disk A: sec=0 if not present
        Devices [1] - disk B: sec=0 if not present
        Devices [2] - disk C: sec=0 if not present
        Devices [3] - disk D: sec=0 if not present
        ...
        */

/* Таблица процессов */
struct processes process_table [PROC_MAX];
/************************** include functions ******************************/
#include "alloc.c"
#include "process.c"
#include "syscall.c"

void test_devices(void);
void interrupt int1b(void);

/**************************** Functions ************************************/

#if 0
void fuck(void)
{
int Divisor [] = {10000, 1000, 100, 10, 1};

printf("fuck = %04X\n",Divisor[0]);

}
#endif

void ident(void)
{
printf("Proolix kernel ");
printf("%s\nCompile by Turbo C 2.01 at %s %s\n",VER,__DATE__,__TIME__);
}

/*----------------------------------------------------------------------------
 ╔══════════════════════════════════════════════════════════════════════════╗
 ║ ╔══════════════════════════════════════════════════════════════════════╗ ║
 ║ ║                                                                      ║ ║
 ║ ║                            Main function                             ║ ║
 ║ ║                                                                      ║ ║
 ║ ╚══════════════════════════════════════════════════════════════════════╝ ║
 ╚══════════════════════════════════════════════════════════════════════════╝
----------------------------------------------------------------------------*/
void main (void)
{int i;
/* int OldSegment, OldOffset; */
Mode=0; /* kernel mode */
User=0;
Multitask=0;

ProcessPar=kernel_begin();

puts("\nKernel started");

test_devices();

init();

ident();

print_mount();

#ifndef NODISK

#if 1
_argv[0]="kernel.ovl";
_argv[1]="\r";
_argv[2]=NULL;
if ((i=execve(_argv[0],_argv,pEnv))!=0) printf("kernel: no overlay. rc=%i ",i);
#endif

#if 0
#ifdef _RC_
_argv[0]=_RC_;
_argv[1]=NULL;
execve(_argv[0],_argv,pEnv);
#endif /* _RC_ */
#endif

#endif /* NODISK */

#ifdef CHANGE_INT_1B
#if 0
setvect(0x1B,(void interrupt(*)(void)) sh );
#else
setvect(0x1B,(void interrupt(*)(void)) int1b );
#endif
#endif

sh();

}