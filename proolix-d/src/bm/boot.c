#define VERSION "Proolix boot manager"

/* #define DEBUG */

/*
History in reverse order
0.0.2.0 27-Aug-2003 - add command "boot"
0.0.1.3 28-May-2003
0.0.1.2 26-Aug-99 - 0.0.1.1, compile by Turbo C 2.01
                    (all previous compiled by TC 2.0)
0.0.1.1 23-Feb-99 - add NumLock off, anykey for interrupt boot sec.
0.0.1.0 10-Mar-98
0.0.0.29 15-Feb-98
0.0.0.28  4-Feb-98
0.0.0.27 18-Aug-97
0.0.0.26 10-Aug-97
0.0.0.25 31-May-97
0.0.0.24 31-May-97
0.0.0.23W 12-May-97 - add command MORE and NOMORE
0.0.0.22 11-May-97 - F1 - boot from 1st HDD
0.0.0.21  6-May-97 - á¤¥« ­  à ¡®â  á ª« áâ¥à ¬¨ ¤® 32Š ¢ª«îç¨â¥«ì­®
0.0.0.20  1-May-97 - fixed case Cylinder>255 in HDD
0.0.0.19 27-Apr-97 - booted from A:, B:, C:, D:, etc...
0.0.0.18 26-Apr-97
0.0.0.17 23-Apr-97
0.0.0.16 22-Apr-97
0.0.0.15 21-Apr-97
0.0.0.14 20-Apr-97 - ®â«®¢«¥­ ¡®«ìè®© £«îª á mov SP, ...
0.0.0.13 19-Apr-97 - add boot.h and AUTOSTART preprocessor variable
0.0.0.12 12-May-96 - ¢ë­¥á Cluster ¢ static
0.0.0.11 21-Apr-96
0.0.0.10 20-Apr-96 - /boot â¥¯¥àì á¦ â PKLite'®¬
0.0.0.9  15-Apr-96 - ¤¥« î § £àã§ªã á «î¡®£® à §¤¥«  ¢¨­ç¥câ¥à  (  ­¥ â®«ìª®
á C: ª ª íâ® ¡ë«® à ­ìè¥)
0.0.0.8  11-Apr-96 - á¤¥« « à ¡®âã á FAT à §¬¥à®¬ ¡®«ìè¥, ç¥¬ 64Š
0.0.0.7   6-Apr-96
0.0.0.7   5-Apr-96
0.0.0.6   4-Apr-96
0.0.0.5  31-Mar-96
0.0.0.5  30-Mar-96 - HDD...
0.0.0.4  27-Mar-96 - add "dump" command
0.0.0.3  25-Mar-96
0.0.0.2  18-Mar-96 - à ¡®â «® â®«ìª® á ª« áâ¥à ¬¨ 1024ª (à ¡®â «® á 3" 720k
¤¨áª¥â ¬¨ ¨ ­¥ à ¡®â «® á 3" 1.2M)
0.0.0.1  13-Mar-96
0.0.0.1  12-Mar-96
0.0.0.1   9-Mar-96
0.0.0.0  27-Feb-96
*/
#include <prool.h>

#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <struct.h>
#include <dirent.h>
#include <string.h>
#include <kbd.h>

#include <conf.c>

#include <proto.h>
#include "boot.h"

#define AUTOSTART
#ifndef DEF_BOOT
#define DEF_BOOT 0 /* default boot device 0 - a: 1 - b: 2 - c: */
#endif

#define CLUSTER_ADDR MK_FP(0x4050, 0)
#define FAT_ADDR MK_FP(0x5050, 0)

#define COMMAND_LINE 80
#define KERNEL_BEGIN_SEG 0x60
#define EXE_HEADER_SIZE 0x1c
#define MAX_CLU_SEC 64 /* 64 */ /* 16 */
#define PSP KERNEL_BEGIN_SEG-0x10

#ifdef _HERCULES_
#define MAIN_COLOR 7
#else
#define MAIN_COLOR 11
#endif

#define ReadPhysSec2 ReadPhysSec

unsigned char NearBuffer [SECTOR_SIZE];
char far * pCluster;
unsigned long RootBeg;
unsigned long RootEnd;
int CurrentDevice;
int FATMode;
struct BootStru *B;
char huge *FAT ;
int NextDrive=2; /* disk C: */
int NextDOSDrive='C';
void kernel_start (void);

struct DeviceStruct Devices [LASTDRIVE];
  /*
        Devices [0] - disk A: sec=0 if not present
        Devices [1] - disk B: sec=0 if not present
        Devices [2] - disk C: sec=0 if not present
        Devices [3] - disk D: sec=0 if not present
        ...
        */
/*--------------------------------------------------------------------------*/
int TextAttr=MAIN_COLOR;
char far *KbdStatus;
char Russian [256];

char Buf  [SECTOR_SIZE];
char Buf2 [SECTOR_SIZE];
char MBRBuf [SECTOR_SIZE];
char command [COMMAND_LINE];

int errno;

int SectorsOnCyl;
unsigned long MaxSectors;
int TrkSecs;
int HeadCnt;
int CluSize;
unsigned int CluSizeBytes;
int FatSize;
int FatCnt;
unsigned long ResSecs;
unsigned long DataStart;
unsigned int TargetForBugs;
unsigned int MaxClusters;
int More=0;
int NLine=0;
/****************************************************************************/
void cold (void);
void hdd2 (void);
void out_os (int c);
void load2go (unsigned char drive, unsigned char sec, unsigned char head,
                 unsigned char trk);  /* load boot sector of partition and go this */

#include "..\kernel\ppt.c"
/****************************************************************************/
void boot_from_partition(unsigned char commandline[])
{
int sec, head, trk, olddevice, oldpart;
unsigned long i;
int part_no, NewDevice;

part_no=commandline[5]-'0';
printf("Booting from partition no %i\n",part_no);

olddevice=CurrentDevice;

CurrentDevice=part_no;
NewDevice=part_no;

if (NewDevice>1)
  { /* HDD */
  if (Devices[NewDevice].sec==0)
    {
    printf("bootman: invalid device\n");
    CurrentDevice=olddevice;
    return;
    }
  head=Devices[NewDevice].head;
  sec=Devices[NewDevice].sec;
  trk=Devices[NewDevice].trk;
  if (ReadPhysSec (_HDD_, sec, head, trk, Buf))
    {
    printf ("boot: partition boot sector read error\n");
    CurrentDevice=olddevice;
    return;
    }
  for (i=0;i<16;i++)printf ("%02X ",Buf[(int)i]);
  puts("press anykey"); getch();
  load2go(_HDD_, sec, head, trk); /* load boot sector of partition and go this */
  }
else
  { /* FDD */
  if (Devices[NewDevice].sec==0)
    {
    printf("bootman: invalid device\n");
    CurrentDevice=olddevice;
    return;
    }
  if (bootread0 (CurrentDevice, Buf))
    {
    printf ("boot: boot sector read error :( \n");
    CurrentDevice=olddevice;
    return;
    }
  B = (void *) Buf;

  #if defined (DEBUG)
  out_boot((void far *) Buf);
  #endif

  }

}
/****************************************************************************/
void ident (void)
{

printf("\n\n%s\n",VERSION);

#ifdef AUTOSTART
printf("Press ESC fastly for interrupt boot sequence\n");
#endif

printf("help - help\n\n\
Enter - load standard Proolix kernel\n\n\
F1 - forced boot from 1st HDD\n\n\
");

}
/****************************************************************************/
void help (void)
{
ident();
printf("\
help - this help\n\
0:\n\
1: - set FDD\n\
2:\n\
3:\n\
... - set HDD\n");
printf("reboot - hot reboot\n");
printf("cold - cold reboot\n");
printf("msdos - load OS from HDD 0\n");
printf("hdd2 - load OS from HDD 1 (if present)\n");
printf("ls - ls\n");
printf("more - set more on\n");
printf("nomore - set more off\n");
printf("dump filename - hex dump of filename\n");
printf("boot <num> - boot from partition number\n\
");
}
/****************************************************************************/
int TR (int curdev)
{
if (curdev>1) return _HDD_; /* HDD */
else return curdev; /* FDD */
}
/****************************************************************************/
void RestoreVec (void)
{
}
/****************************************************************************/
int sema_lock (void /* ;) */ )
{
return 0;
}
/****************************************************************************/
void sema_unlock (void /* ;) */ )
{
}
/****************************************************************************/
int putch__(char c)
{
switch (c)
  {
  case 0: c=' '; break;
  default: if (c<' ') c='.';
  }
return putch(c);
}

/****************************************************************************/
#ifdef DEBUG
void out_boot(void far *buf)

{int i;
unsigned long DiskSize;
unsigned long TrueSectors;
struct BootStru far *b;
b=buf;
printf("\n\
----------------------------------Disk parameters:------------------------------\n");
printf("Jump command %02X %02X %02X  \n",(*b).Jmp[0],(*b).Jmp[1],(*b).Jmp[2]);
printf("OEM name     %-8s\n\
OEM name     ",(*b).OEM);

for(i=0;i<8;) printf("%02X ",(*b).OEM[i++]);

#if 1
printf("\nSector size                      %4i bytes Cluster size                %1i sect\n",(*b).SectSiz,(*b).ClustSiz);
printf("Reserved sectors (before 1st FAT)  %2i       FAT counter                 %1i\n",(*b).ResSecs,(*b).FatCnt);
printf("Root directory entries           %4i       Total sectors         %7u\n",(*b).RootSiz,(*b).TotSecs);
printf("Media descr                        %02X       FAT size                %5i sect\n",(*b).Media,(*b).FatSize);
printf("Track size                         %2i sec   Heads                      %2i\n",(*b).TrkSecs,(*b).HeadCnt);
printf("Hidden sectors                %7li       Big Number Of Sectors %7li\n",(*b).HidnSec,(*b).BigSect);
printf("Physical Drive No                  %02X       Extended Boot Signature    %02X\n",(*b).DriveNo,(*b).BootSign);
printf("Volume Serial No %04X-%04X\n",
(*b).SerialNo[1],
(*b).SerialNo[0]);
#endif

printf("\nVolume Label (in boot)  ");
for(i=0;i<11;)putch__((*b).VolLbl[i++]);
printf("\n                        ");
for(i=0;i<11;)printf("%02X ",(*b).VolLbl[i++]);
printf("\nFile system Id          ");
for(i=0;i<8;)putch__((*b).FileSysId[i++]);
printf("\n                        ");
for(i=0;i<8;)printf("%02X ",(*b).FileSysId[i++]);

if ((*b).TotSecs==0) TrueSectors=(*b).BigSect;
else TrueSectors=(*b).TotSecs;

DiskSize=((long) (*b).SectSiz * TrueSectors)/1024l;

if (DiskSize>5000)
  {
  DiskSize/=1024;
  printf("\nDisk size %li Mb\n",DiskSize);
  }
else
  {
  printf("\nDisk size %li Kb\n",DiskSize);
  }

}
#endif
/****************************************************************************/
void setdisk (int NewDevice)
{int sec, head, trk, olddevice, oldpart;
unsigned long i;

olddevice=CurrentDevice;

CurrentDevice=NewDevice;

if (NewDevice>1)
  { /* HDD */
  if (Devices[NewDevice].sec==0)
    {
    printf("bootman: invalid device\n");
    CurrentDevice=olddevice;
    return;
    }
  head=Devices[NewDevice].head;
  sec=Devices[NewDevice].sec;
  trk=Devices[NewDevice].trk;
  if (ReadPhysSec (_HDD_, sec, head, trk, Buf))
    {
    printf ("boot: partition boot sector read error\n");
    CurrentDevice=olddevice;
    return;
    }
  /* printf("========================================================\n"); */
  B = (void *) Buf;
  #if 0
  out_boot((void far *) Buf);
  #endif

  HeadCnt=B->HeadCnt;
  TrkSecs=B->TrkSecs;
  SectorsOnCyl=HeadCnt*TrkSecs;

  #if 0
  ResSecs=(unsigned long)Devices[NewDevice].sec+
    (unsigned long)Devices[NewDevice].head*(unsigned long)TrkSecs+
    (unsigned long)Devices[NewDevice].trk*SectorsOnCyl;
  #else
  ResSecs=(unsigned long)(Devices[NewDevice].sec & 0x3F)+
    (unsigned long)Devices[NewDevice].head*(unsigned long)TrkSecs+
    (unsigned long)(Devices[NewDevice].trk|((int)(Devices[NewDevice].sec & 0xC0)<<2))*
    SectorsOnCyl;
  #endif

  #ifdef DEBUG
  printf("ResSecs=%lu\n",ResSecs);
  #endif

  MaxSectors=Devices[NewDevice].MaxSectors+ResSecs;
  FATMode=Devices[NewDevice].FileSystem;

  }
else
  { /* FDD */
  if (Devices[NewDevice].sec==0)
    {
    printf("bootman: invalid device\n");
    CurrentDevice=olddevice;
    return;
    }
  if (bootread0 (CurrentDevice, Buf))
    {
    printf ("boot: boot sector read error :( \n");
    CurrentDevice=olddevice;
    return;
    }
  B = (void *) Buf;

  #if defined (DEBUG)
  out_boot((void far *) Buf);
  #endif

  if ((i=B->ResSecs)!=0) ResSecs=i; ResSecs=B->HidnSec+1;
  if ((i=B->TotSecs)!=0) MaxSectors=i;
  else MaxSectors=B->BigSect+B->HidnSec;

  #ifdef DEBUG
  printf("MaxSectors=%li\n",MaxSectors);
  #endif

  HeadCnt=B->HeadCnt;
  TrkSecs=B->TrkSecs;
  SectorsOnCyl=HeadCnt*TrkSecs;
  FATMode=FAT12;
  }

CluSize=B->ClustSiz;

if (CluSize>MAX_CLU_SEC)
printf("\nPANIC!!! Cluster size=%i sec - is too large for this realization\n\n\
Max supported cluster size=%i sec\n\n",CluSize,MAX_CLU_SEC);

CluSizeBytes=CluSize * SECTOR_SIZE;
FatSize=B->FatSize;
FatCnt=B->FatCnt;
RootBeg = ResSecs + B->FatCnt * B->FatSize;
RootEnd = RootBeg + ( ( B->RootSiz * 32 ) / B->SectSiz ) - 1;
DataStart = RootEnd+1;
MaxClusters=(unsigned int)((MaxSectors-DataStart)/CluSize+1);

#ifdef DEBUG
printf("RootBeg =%lu\n",RootBeg);
printf("RootEnd =%lu ",RootEnd);
printf("ResSecs =%lu ",ResSecs);
printf("MaxSectors =%lu =%08lX MaxClusters =%u\n",
MaxSectors,MaxSectors,MaxClusters);
printf("\nHeadCnt =%2i TrkSecs =%2i FatSize =%2i FatCnt =%1i\n",
HeadCnt,TrkSecs,FatSize,FatCnt);
printf("B->SectSiz =%i ", B->SectSiz );
printf("B->ClustSiz=%i ", B->ClustSiz);
printf("B->ResSecs =%u ", B->ResSecs );
printf("B->FatCnt  =%i ", B->FatCnt  );
printf("B->RootSiz =%i ", B->RootSiz );
printf("B->TotSecs =%u ", B->TotSecs );
printf("B->Media   =%02X ", B->Media );
printf("B->FatSize =%i ", B->FatSize );
printf("B->TrkSecs =%i ", B->TrkSecs );
printf("B->HeadCnt =%i ", B->HeadCnt );
printf("B->HidnSec =%lu ",B->HidnSec );
printf("B->BigSect =%lu ",B->BigSect );
printf("B->DriveNo =%02X ", B->DriveNo );
printf("B->BootSign=%02X ", B->BootSign);
printf("DataStart = %lu ",DataStart);
printf("\nCluster size = %u bytes\n",CluSizeBytes);
#endif

}
/****************************************************************************/
void ls (void)
{int i, k;
unsigned long j;
struct dirent16 *Ent;

for (j=RootBeg;j<=RootEnd;j++)
  {
  if (absread (TR(CurrentDevice), 1, j, Buf2))
    printf ("boot: sector %lu absread error\n", j);
  else
    {
    for (k=0;k<16;k++)
      {
      Ent = (void *) (Buf2+k*32);
      if ((*Ent).d_name[0]==0) continue;
      if ((*Ent).d_name[0]==0xE5) continue;
      #if 0
      if ((*Ent).Attr.B.Label) continue;
      if ((*Ent).Attr.B.Dir) continue;
      #endif
      for (i=0;i<11; i++) printf("%c",Ent->d_name[i]);
      printf(" %u\n",Ent->d_fileno);
      }
    }
  }
}
/****************************************************************************/
void load_and_run (char *KernelName)
{int i, k;
unsigned int Clu;
unsigned long j;
unsigned len, len1;
struct dirent16 *Ent;
char DirectoryRecord [11];
char huge * Adr;
struct exe_header far * H;
unsigned int far * Rel;
unsigned int I_OFF, I_SEG, RELO_SEG;

for (i=0;i<11;i++)DirectoryRecord[i]=' ';
memcpy(DirectoryRecord,KernelName,strlen(KernelName));

for (j=RootBeg;j<=RootEnd;j++)
  if (absread (TR(CurrentDevice), 1, j, Buf2))
    printf ("boot: sector %lu absread error\n", j);
  else
    {
    for (k=0;k<16;k++)
      {
      Ent = (void *) (Buf2+k*32);
      if (Ent->d_name[0] && ((*Ent).d_name[0]!=0xE5))
        {
        if (Ent->Attr.B.Label) continue;
        if (Ent->Attr.B.Dir) continue;
        if (memcmp(Ent->d_name,DirectoryRecord,11)==0)
          {
          printf("Kernel `%s' found. First cluster=%u Size=%lu\n",
          KernelName,Ent->d_fileno,Ent->Size);
          printf("Loading to %04X:0000\n",KERNEL_BEGIN_SEG);
          /* Load FAT */
          j=ResSecs;
          FAT = FAT_ADDR;
          #if 1
          for (i=0; i<FatSize; i++)
            {
            absread(TR(CurrentDevice), 1, j++, (char far *) FAT);
            FAT += SECTOR_SIZE;
            }
          #endif
          FAT = FAT_ADDR;
          Clu = Ent->d_fileno;
          Adr = MK_FP (KERNEL_BEGIN_SEG,0);

          CluRead2(TR(CurrentDevice),Clu,(char far *)Adr);

          if (memcmp((char far *)Adr,"MZ",2))
            { /* COM */
            printf("COM KERNEL\n");
            Adr += CluSizeBytes;
            Clu=NextClu2(Clu);

            while (Clu!=0xFFFFU)
              {
              CluRead2(TR(CurrentDevice),Clu,(char far *)Adr);
              putch('#');
              Adr += CluSizeBytes;
              Clu=NextClu2(Clu);
              };

            kernel_start();
            }
          else
            {
            #if 1
            memcpy(pCluster,(char far *)Adr,CluSizeBytes);
            #else
            {int ii;
            for (ii=0;ii<CluSizeBytes;ii++)
              *(pCluster+ii)=*((char far *)Adr+ii);
            }
            #endif
            H=(void far *)pCluster;
            ViewEXE(H);
            #ifdef DEBUG
            for (i=0;i<7;i++) printf("2345678901");
            puts("");
            #endif
            if ((len1=H->HdrSize) > 0x20)
              {printf ("boot: illegal HdrSize = %04X",len1); goto l1; }
            /* § £àã§ª  ä ©«  ¢ Ž‡“ */
            Adr = MK_FP (KERNEL_BEGIN_SEG,0);
            #if 1
            memcpy((char far *)Adr,pCluster+(len1<<4),
                   len=CluSizeBytes-(len1<<4));
            #else
               {int ii, jj;
               jj=len1<<4;
               for (ii=0;ii<(len=CluSizeBytes-jj);ii++)
                 *(Adr+ii)=*(pCluster+jj+ii);
               }
            #endif
            Adr += len;
            Clu=NextClu2(Clu);

            #if !defined(_HERCULES_)
            textattr(9);
            #endif

            while (Clu!=0xFFFFU)
              {
              CluRead2(TR(CurrentDevice),Clu,(char far *)Adr);
              #ifdef DEBUG
              putch('#'); /* printf(" %08lX",Adr); */
              #endif
              Adr += CluSizeBytes;
              Clu=NextClu2(Clu);
              };
            /* ­ áâà®©ª  ¯¥à¥¬¥é ¥¬ëå  ¤à¥á®¢ */
            puts("");
            Rel=(unsigned int far *)(pCluster + (H->TablOff));
            for (i=0; i<H->ReloCnt; i++)
              {
              /* printf(" #%02X",i); */
              I_OFF=*Rel++;
              /* printf(" I_OFF=%04X",I_OFF); */
              I_SEG=*Rel++;
              /* printf(" I_SEG=%04X",I_SEG); */
              RELO_SEG=KERNEL_BEGIN_SEG+I_SEG;
              /* printf(" RELO_SEG=%04X",RELO_SEG); */
              *(unsigned int far *)MK_FP(RELO_SEG,I_OFF) =
                *(unsigned int far *)MK_FP(RELO_SEG,I_OFF)+KERNEL_BEGIN_SEG;
              putch('@');
              }
            #if !defined(_HERCULES_)
            textattr(4);
            #endif
            puts("\nEXEC! ");
            *(int far *)MK_FP(PSP,0)=0x20CD; /* INT 20H :) */
            *(int far *)MK_FP(PSP,2)=0x9FFF; /* ¤«ï LZEXE */
            *(int far *)MK_FP(PSP,4)=CurrentDevice;
            /* EXEC ! */
            /* ES = DS = PSP */
            /* SS = START_SEG+ReloSS, SP = ExeSP */
            /* CS = START_SEG+ReloCS, IP = ExeIP */
            /* (ª®¬ ­¤ë: PUSH seg; PUSH offset; RETF) */
            i=H->ReloSS+KERNEL_BEGIN_SEG;
            asm mov cx,i;
            i=H->ReloCS+KERNEL_BEGIN_SEG;
            asm mov dx,i;
            i=H->ExeSP;
            asm mov si,i;
            i=H->ExeIP;
            asm mov di,i;

            asm mov bx,PSP;
            asm mov DS,bx;
            asm mov ES,bx;

            asm cli;
            asm mov SS,cx;
            asm mov SP,si;
            asm sti;

            asm push dx;
            asm push di;
            asm retf;
            }
          }
        }
      }
    }
printf("Kernel `%s' not found\n",KernelName);
l1:;
}
/****************************************************************************/
void dump (char far *Command)
{int i, k;
unsigned int Clu;
unsigned long j;
unsigned len, len1;
struct dirent16 *Ent;
char DirectoryRecord [11];
char far *FileName;
char far *ppp;

if ((FileName=strchr(Command,' '))==NULL) return;
FileName++;
if (*FileName==0) return;

printf("Dump %Fs\n",FileName);

for (i=0;i<11;i++)DirectoryRecord[i]=' ';
memcpy(DirectoryRecord,FileName,strlen(FileName));

for (j=RootBeg;j<=RootEnd;j++)
  if (absread (TR(CurrentDevice), 1, j, Buf2))
    printf ("boot: sector %lu absread error\n", j);
  else
    {
    for (k=0;k<16;k++)
      {
      Ent = (void *) (Buf2+k*32);
      if ((*Ent).d_name[0] && ((*Ent).d_name[0]!=0xE5))
        {
        if ((*Ent).Attr.B.Label) continue;
        if ((*Ent).Attr.B.Dir) continue;
        if (memcmp(Ent->d_name,DirectoryRecord,11)==0)
          {
          printf("File `%Fs' found. Cluster %u\n",FileName,Ent->d_fileno);
          #ifdef DEBUG
          printf(" press any key ");
          getch();
          #endif
          /* Load FAT */
          j=ResSecs;
          FAT = FAT_ADDR;
          for (i=0; i<FatSize; i++)
            {
            absread(TR(CurrentDevice), 1, j++, (char far *) FAT);
            FAT += SECTOR_SIZE;
            }
          FAT = FAT_ADDR;
          Clu = Ent->d_fileno;
          #ifdef DEBUG
          printf("FAT loaded\n");
          printf(" press any key ");
          getch();
          #endif
          while (Clu!=-1U)
            {
            CluRead2(TR(CurrentDevice),Clu,pCluster);
            ppp=pCluster;
            for (i=0;i<CluSize;i++)
              {
              for (j=0; j<SECTOR_SIZE; j++)
                {char c;
                c=*ppp++;
                if (c<' ') /* putch('.') */ ;
                else putch(c);
                }
              #ifdef DEBUG
              printf(" press any key for next sector ");
              getch();
              #endif
              }
            Clu=NextClu2(Clu);
            };
          return;
          }
        }
      }
    }
printf("File `%s' not found\n",FileName);
l1:;
}
/****************************************************************************/
void AntiVirus(void)
{
if (((unsigned)FP_SEG(getvect(0x13)))<0xC000U)
  {
  printf("WARNING: Maybe, boot virus. Int 13h vector changed!!! ");
  OutIntVector(0x13);
  printf("Press any key and use antiviral software\n");
  getch();
  }
else
  /* printf("\n no vir\n") */ ;
}
/****************************************************************************/
void test_devices (void)    /* á¬.  ­ «®£¨ç­ãî äã­ªæ¨î test_devices() ¢      */
{unsigned int i;           /* ï¤à¥ src/kernel/k2.c                          */
struct MBRstru *MBR;

for (i=0;i<LASTDRIVE;i++)Devices[i].sec=0;

/* ˆ§ [Tech Help]

ÉÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ»
º                             ‘¯¨á®ª ®¡®àã¤®¢ ­¨ï                            º
ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ¼
â®â á¯¨á®ª ®¯¨áë¢ ¥â ãáâ ­®¢«¥­­®¥ ¨«¨  ªâ¨¢­®¥ ®¡®àã¤®¢ ­¨¥. Ž­ ¢®§¢à é ¥âáï
¯à¥àë¢ ­¨¥¬ INT 11H (¢ AX) ¨ åà ­¨âáï áà¥¤¨ ¤ ­­ëå BIOS ¯®  ¤à¥áã 0:0410.
 1 1 1 1 1 1
Ö5Â4Â3Â2Â1Â0Â9Â8Ò7Â6Â5Â4Â3Â2Â1Â0·  ‚ë§®¢¨â¥ INT 11H ¨«¨ ¨§¢«¥ª¨â¥ á«®¢® 0:0410
ºprt³ ³j³ aux ³ ºdrv³vid³RAM³ ³ º
ÓÄÁÄÁÒÁÒÁÄÁÄÁÄÁÒÐÄÁÄÁÄÁÄÁÄÁÄÁÒÁÒ½ ¡¨â                                  ¬ áª 
 ÈË¼ º º ÈÍËÍ¼ º ÈË¼ ÈË¼ ÈË¼ º ÈÍ 0: 0 = ­¥â ¤¨áª®¢ëå ãáâà®©áâ¢    (AX & 0001H)
  º  º º   º   º  º   º   º  ÈÍÍÍ 1: 8087 á®¯à®æ¥áá®à (­¥­ ¤¥¦­®)
  º  º º   º   º  º   º   ÈÍÍÍÍÍÍ 2-3: ¬ â¥à¨­áª ï ¯« â  RAM       (AX & 0eH)
  º  º º   º   º  º   º                 01H=16K; 04H=32K; 0eH=64K (¨«¨ ¡®«ìè¥)
  º  º º   º   º  º   ÈÍÍÍÍÍÍÍÍÍÍ 4-5: ­ ç «ì­ë©/ ªâ¨¢­ë© à¥¦¨¬    (AX & 0030H)
  º  º º   º   º  º                     10H=40-ª®« æ¢¥â­; 20H=80-ª®« æ¢¥â­;
  º  º º   º   º  º                     30H=TTL ¬®­®åà®¬­ ï ¯« â 
  º  º º   º   º  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍ 6-7: ¢á¥£® ¤¨áª®¢®¤®¢ -1         (AX & 00c0H)
  º  º º   º   º                        00H=1; 40H=2; 80H=3; c0H=4
  º  º º   º   ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ 8: DMA ¯à¨áãâáâ¢ã¥â              (AX & 0100H)
  º  º º   ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ 9-11: RS232 ¯®àâë                (AX & 0e00H)
  º  º º                                 000H=­¥â; 200H=1; 400H=2; ... e00H=7
  º  º ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ12: 1=¨£à®¢®©  ¤ ¯â¥à ¯à¨áãâáâ¢ã¥â(AX & 1000H)
  º  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ13: 1=¯®á«¥¤.¯à¨­â¥à  Jr â®«ìª®
  ÈÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ14-15: ãáâ ­®¢«¥­® ¯à¨­â¥à®¢      (AX & c000H)
                                         0000H=­¥â; 4000H=1; 8000H=2; c000H=3

‡ ¬¥ç: — áâ® ¨á¯®«ì§ã¥âáï ¤«ï ¯à®¢¥àª¨ à¥¦¨¬  ¢¨¤¥®. …á«¨ (AX & 30H) = 30H, â®
        ¤ ¯â¥à ¬®­®åà®¬­ë©ÄÄ¢¨¤¥® á¥£¬¥­â RAM ¯®  ¤à¥áã 0b000H, ¨­ ç¥ 0b800H.

*/

i=((*(unsigned int far *)MK_FP(0,0x410) & 0xC0)>>6)+1;

if (i) Devices[0].sec=-1;

if (i>1) Devices[1].sec=-1;

if (*(char far *)MK_FP(0,0x475)) /* 40h:75h - number of fixed disk drives */
  if (bootread0 (_HDD_, MBRBuf)) /*                      [Interrupt List] */
    printf ("boot: MBR read error\n");
  else
    {
    /* printf("# head sec trk ResSecs MaxSectors\n"); */
    process_partition_table (MBRBuf);
    }
else ;

/* outMBR(MBRBuf); */
}
/****************************************************************************/
char *gets (char *s)
{
int i;
char buf[MAX_LEN_STR];
char c;

for(i=0;i<MAX_LEN_STR;)
  {
  c=getchar();

  #if 0
  if (putchar(c)==EOF)
                  {buf[i]=0; buf[MAX_LEN_STR-1]=0; return NULL;}
  #endif

  switch (c)
    {
    case 0: /* extended code */
            c=getchar();
            switch (c)
              {
              case ';': /* F1 */
                        printf(" boot from 1st HDD: ");
                        strcpy(s,"MSDOS");
                        return s;
              default : ;
              }
            break;
    case '\b' : /* ERASE=8 */
                if (i)
                  {
                  buf[--i]=0;
                  putch('\b');
                  putch(' ');
                  putch('\b');
                  }
                else
                  buf[i]=0;
                break;
    case EOF  :
    case   4  : /* CTRL D */
    case '\r' : /* CR */
    case '\n' : /* LF */
                buf[i]=0; i=MAX_LEN_STR; break;
    default:    buf[i++]=c;
    }
  }

buf[MAX_LEN_STR-1]=0;

#ifdef DEBUG
printf(" gets:buf=",buf);
puts(buf);
#endif

#if 0
strcpy(s,buf);
#endif

#if 1
for (i=0;i<MAX_LEN_STR;i++)
  {
  *s++=buf[i];
  if (buf[i]==0) break;
  }
#endif

#ifdef DEBUG
printf(" gets:s=");
puts(s);
#endif

return s;
}
/****************************************************************************/
void main (void)
{int i, j, k;
struct dirent16 *Ent;

for (i=0;i<256;i++) Russian[i]=i;

ident();

#if 1
/* Numlock off */
asm mov ax,40h;
asm mov es,ax; /* ES:=40h */
asm and byte ptr es:17h,0DFh;
#endif

pCluster=CLUSTER_ADDR;

AntiVirus();

for (i=0;i<LASTDRIVE;i++) Devices[i].sec=0;

test_devices();

CurrentDevice=-1;

setdisk(DEF_BOOT);

#ifdef AUTOSTART
  #if 0
  if ((*(char far *)MK_FP(0x40,0x17) & R_SHIFT) == 0)
  #else
  if (kbhit()==0)
  #endif
load_and_run ("KERNEL");
#endif

while (kbhit()) getch();

while (1)
  {
  if (CurrentDevice==-1) putch('?'); else putch('0'+CurrentDevice);
  putch('>');
  gets(command);
  strupr(command);
  /* printf("\ncom=`%s'\n",command); */
  if (command[0]==0) { load_and_run ("KERNEL"); }
  else if (strcmp(command,"HELP")==0) help();
  else if (strcmp(command,"REBOOT")==0) reboot();
  else if (strcmp(command,"COLD")==0) cold();
  else if (strcmp(command,"MSDOS")==0) msdos();
  else if (strcmp(command,"HDD2")==0) hdd2();
  else if (strcmp(command,"LS")==0) ls();
  else if (strcmp(command,"MORE")==0) More=1;
  else if (strcmp(command,"NOMORE")==0) More=0;
  else if (memcmp(command,"DUMP ",5)==0) dump(command);
  else if (memcmp(command,"BOOT ",5)==0) boot_from_partition(command);
  else if (command[1]==':')
    {
    setdisk(command[0]-'0');
    }
  else { load_and_run (command); }
  }
}