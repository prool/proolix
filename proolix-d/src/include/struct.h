/*      struct.h

        Proolix Kernel structures (struct)

        Copyright (c) Serge Pustovoitoff 1994, 1995

*/

#ifndef LIMITS_H
#include "limits.h"
#endif

#ifndef _IO_H
#include <io.h>
#endif

#if     !defined(__TIME_DATE_)
#define __TIME_DATE_

struct  time    {
        unsigned char   ti_min;         /* Minutes */
        unsigned char   ti_hour;        /* Hours */
        unsigned char   ti_hund;        /* Hundredths of seconds */
        unsigned char   ti_sec;         /* Seconds */
};

struct  date    {
        int             da_year;        /* Year - 1980 */
        char            da_day;         /* Day of the month */
        char            da_mon;         /* Month (1 = Jan) */
};
#endif

#ifndef STRUCT_H
#define STRUCT_H

struct MemBlk
{
char Type;
unsigned int  OwnerPar;
unsigned int  SizePar;
};

/*
struct TimeField
{
unsigned int Sec2     : 5;
unsigned int Min      : 6;
unsigned int Hour     : 5;
};

struct DateField
{
unsigned int Day      : 5;
unsigned int Month    : 4;
unsigned int Year     : 7;
};
  */

struct BootStru /* structure of boot sector of FAT12 and FAT16*/
  {
  unsigned char Jmp [3];
  unsigned char OEM [8];
  int  SectSiz;
  char ClustSiz;
  int  ResSecs;
  char FatCnt;
  int  RootSiz;
  unsigned int  TotSecs;
  unsigned char Media;
  int  FatSize;
  int  TrkSecs;
  int  HeadCnt;
  unsigned long HidnSec;
  unsigned long BigSect;
  unsigned char DriveNo;
  unsigned char Thing;
  unsigned char BootSign;
  int  SerialNo [2];
  unsigned char VolLbl [11];
  unsigned char FileSysId [8];
  /*
  int  FATaddr;
  int  StartClu;
  int  RootSize;
  */
  };

struct BootStru32 /* structure of boot sector of FAT32*/
  {
  unsigned char Jmp [3];
  unsigned char OEM [8];
  int  SectSiz;
  char ClustSiz;
  int  ResSecs;
  char FatCnt;
  int  RootSiz;
  unsigned int  TotSecs;
  unsigned char Media;
  int  FatSize;
  int  TrkSecs;
  int  HeadCnt;
  unsigned long HidnSec;
  unsigned long BigSect;
  unsigned long BigFATSize;
  unsigned int ExtendedFlags;
  unsigned int FilesystemVer;
  unsigned long RootBegClu;
  unsigned int AuxSector;
  unsigned int CopySector;
  unsigned char Reserved[12];
  unsigned char String[8];
  };

struct Partition_stru
  {
  unsigned char   indicator;
        /*
        00  - non-bootable partition
        80  - bootable partition (one partition only)
        */
  unsigned char   begin_head;
  unsigned char   begin_sec; /* and 2 high bits of cylinder # */
  unsigned char   begin_cyl; /* low order bits of cylinder # */
        /*
        2 bytes are combined to a word similar to INT 13:

        ³7³6³5³4³3³2³1³0³ 1st byte  (sector)
         ³ ³ ÀÄÁÄÁÄÁÄÁÄÁÄÄ Sector offset within cylinder
         ÀÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄ High order bits of cylinder #

        ³7³6³5³4³3³2³1³0³ 2nd byte  (cylinder)
         ÀÄÁÄÁÄÁÄÁÄÁÄÁÄÄÄÄÄ Low order bits of cylinder #
         */
  unsigned char   system_indicator;
  unsigned char   end_head;
  unsigned char   end_sec; /* and 2 high bits of cylinder # */
  unsigned char   end_cyl; /* low order bits of cylinder # */
  unsigned long   preceding_sec;
  unsigned long   total_sec;
  };

struct MBRstru
  {
  char master_boot_loader [0x1BE];
  struct Partition_stru Partition [4];
  unsigned int Signature; /* 55AA */
  };

struct DevRecord
  {
  char *Name;
  int   Major;
  int   Minor;
  char  Byte; /* 1 - byte-oriented device, 0 - block-oriented */
  };

struct DevByte
  {
  void (*dev_open)  ();
  void (*dev_close) ();
  void (*dev_read)  ();
  void (*dev_write) ();
  void (*dev_ioctl) ();
  };

struct DevBlock
  {
  void (*dev_open)     ();
  void (*dev_close)    ();
  void (*dev_strategy) ();
  };

/* ‘âàãªâãà  â ¡«¨æë ¯à®æ¥áá®¢ */
#include <setjmp.h>
struct processes
  {
  int pid;
  int status; /* 1 -  ªâ¨¢¥­, 0 - ­¥ ªâ¨¢¥­ (¨«¨ ¦¤¥â) */
  int wait; /* pid ¯à®æ¥áá  (®âæ ), ª®â®àë© ¦¤¥â ®ª®­ç ­¨ï íâ®£® ¯à®æ¥áá 
               ¨«¨ 0, ¥á«¨ ­¨ªâ® ­¥ ¦¤¥â */
  jmp_buf control_point;
  char huge *mem0;
  };

/* í«¥¬¥­â ¡ãä¥à­®£® ¯ã«  */
struct BufPool
  {
  unsigned long Sec;
  unsigned int Status; /* ‘â âãá ¡ãä¥à :
    0 - á¢®¡®¤­ë©, 1 - § ¯®«­¥­­ë©, 2 - ¨§¬¥­¥­­ë© */
  unsigned int Counter;
  unsigned char M [SECTOR_SIZE];
  };

struct reloc
  {
  unsigned int offset;
  unsigned int segment;
  };

/* § £®«®¢®ª ¨á¯®«­ï¥¬®£® EXE ä ©«  (ª ª ¢ MSDOS) */
struct exe_header
{                     /* ª®¬¬¥­â à¨¨ ¨§ [TECH Help!] */
char MZ[2];           /* "¯®¤¯¨áì" ä ©«  .EXE ('MZ') */
unsigned int PartPag; /* ¤«¨­  ­¥¯®«­®© ¯®á«¥¤­¥© áâà ­¨æë (®¡ëç­® ¨£­®à¨àã¥âáï) */
unsigned int PageCnt; /* ¤«¨­  ®¡à §  ¢ 512-¡ ©â®¢ëå áâà ­¨æ å, ¢ª«îç ï § £®«®¢®ª */
unsigned int ReloCnt; /* ç¨á«® í«¥¬¥­â®¢ ¢ â ¡«¨æ¥ ¯¥à¥¬¥é¥­¨ï */
unsigned int HdrSize; /* ¤«¨­  § £®«®¢ª  ¢ 16-¡ ©â®¢ëå ¯ à £à ä å */
unsigned int MinMem;  /* ¬¨­¨¬ã¬ âà¥¡ã¥¬®© ¯ ¬ïâ¨ §  ª®­æ®¬ ¯à®£à ¬¬ë (¯ à £à äë) */
unsigned int MaxMem;  /* ¬ ªá¨¬ã¬ âà¥¡ã¥¬®© ¯ ¬ïâ¨ §  ª®­æ®¬ ¯à®£à ¬¬ë (¯ à £à äë) */
unsigned int ReloSS;  /* á¥£¬¥­â­®¥ á¬¥é¥­¨¥ á¥£¬¥­â  áâ¥ª  (¤«ï ãáâ ­®¢ª¨ SS) */
unsigned int ExeSP;   /* §­ ç¥­¨¥ à¥£¨áâà  SP (ãª § â¥«ï áâ¥ª ) ¯à¨ § ¯ãáª¥ */
unsigned int ChkSum;  /* ª®­âà®«ì­ ï áã¬¬  (®âà¨æ â¥«ì­ ï áã¬¬  ¢á¥å á«®¢ ¢ ä ©«¥) */
unsigned int ExeIP;   /* §­ ç¥­¨¥ à¥£¨áâà  IP (ãª § â¥«ï ª®¬ ­¤) ¯à¨ § ¯ãáª¥ */
unsigned int ReloCS;  /* á¥£¬¥­â­®¥ á¬¥é¥­¨¥ ª®¤®¢®£® á¥£¬¥­â  (¤«ï ãáâ ­®¢ª¨ CS) */
unsigned int TablOff; /* á¬¥é¥­¨¥ ¢ ä ©«¥ 1-£® í«¥¬¥­â  ¯¥à¥¬¥é¥­¨ï (ç áâ® 001cH) */
unsigned int Overlay; /* ­®¬¥à ®¢¥à«¥ï (0 ¤«ï £« ¢­®£® ¬®¤ã«ï) */
struct reloc RelTbl [];
};

struct DeviceStruct
  {
  char FileSystem;
  int head;
  int sec;
  int trk;
  unsigned long ResSecs;
  unsigned long MaxSectors;
  char dos_disk; /* 'A' 'B' 'C' 'D' ... */
  char system_indicator;
  };

#endif