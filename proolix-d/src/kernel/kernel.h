/* Include file for external c-functions for kernel (as memd.c)
*/

#include <struct.h> /* structures */

#include <alloc.h>
#include <stdio.h>
#include <dir.h>
#include <dirent.h>
#include <string.h>
#include <mem.h>
#include <ctype.h>
#include <stdlib.h>
#include <io.h>
#include <dos.h>
#include <sys/types.h>
#include <conio.h>
#include <process.h>
#include <fcntl.h>
#include <errno.h>

#include <proto.h>

/******************************** External variables ************************/
#ifdef MOUSE
extern unsigned int PORT;
#endif

/* См. описание этих переменных в файле kernel.c */
extern int Mode, User, Multitask;

extern int HeadCnt, TrkSecs;
extern unsigned long MaxSectors;
extern unsigned int ProcessPar;
extern struct {unsigned int ParNo; void (*Entry)(void);} SysTable [];

extern unsigned long RootBeg, RootEnd, ResSecs;
extern unsigned long RootBegClu;
extern unsigned int CluSize;
extern unsigned long DataStart;
extern unsigned long MaxClusters;

extern char Russian [];
extern struct BufPool far *Cache;

extern struct DevRecord Dev [];
extern FILE FCBS [OPEN_MAX];

extern unsigned long CurDirClu;
extern int Trace;
extern int _argc;
extern char far *_argv [];
extern char far * far *pEnv;
extern char far *hEnv;
extern int curX, curY, maxX, maxY;
extern char far *curAddr;
extern char VideoAttrib;
extern int CurrentDevice;
extern int ReadOnly;
extern int FATMode;
extern int msdos_verbose;
extern int SectorsOnCyl;
extern unsigned long FatSize;
extern int FatCnt;
extern char MasterBootRecord [SECTOR_SIZE];
extern unsigned int CluSizeBytes;
extern char far *OldInt;
extern char far *KbdStatus;
extern unsigned int MemTop;
extern char Russian [256];
extern void interrupt (*OldTimerVec) (void);
extern void far * Fin;
extern struct DeviceStruct Devices [];
extern int NextDOSDrive;
extern struct processes process_table [PROC_MAX];
extern user_ss;
extern user_sp;
extern int emulation_mode;
extern jmp_buf the_return;

        /* Глобальные переменные, доступные из юзерских процессов */
extern int errno;
extern int More;
extern int NLine;

#define _HDD_ 0x80
#define EXTEND 5