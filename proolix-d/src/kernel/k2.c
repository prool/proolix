#include <prool.h>

#include <conf.c>
#include "kernel.h"

/* #define DEBUG */
/* #define PRINTF_DEBUG */
int NextDrive=2; /* disk C: */
void out_boot(void far *buf);
void out_boot32(void far *buf);
/****************************************************************************/
#ifdef PRINTF_DEBUG
char far *far_global_init = "far_global_init";
char far *far_global_uninit;
char *near_global_init = "near_global_init";
char *near_global_uninit;
char global_array_init [] = "global_array_init";
char global_array_uninit [100];
#endif

#define EXTEND 5
#define EXTEND2 0xF

#include "ppt.c"
/****************************************************************************/
int shutdown(void)
{int i;
if ((i=FlushAll())!=0) puts("flush error");
return i;
}
/****************************************************************************/
int LinkClu (unsigned long CluNo, unsigned long NeXT)
{
int nsect, offset;
long fat_byte_no;
int b1, b2, b_next;
unsigned char far *cc;
unsigned int far *ii;

if (CluNo>MaxClusters)
  {puts("LinkClu: Invalid cluster number"); return -1;}

switch (FATMode)
{
case FAT12:
  /* ®¯à¥¤¥«ï¥¬ ­®¬¥à ¡ ©â  ¢ FAT */
  fat_byte_no=(CluNo*3)/2;
  /* ®¯à¥¤¥«ï¥¬ ­®¬¥à á¥ªâ®à  FAT */
  nsect=(int)(fat_byte_no/SECTOR_SIZE);
  /* nsect-®â­®á¨â¥«ì­ë© ­®¬¥à á¥ªâ®à  FAT. 0 - ¯¥à¢ë© á¥ªâ®à FAT */
  if (nsect>=FatSize) {puts("LinkClu: Invalid FAT's computing"); return -1;}
  if ( (b1=LoadCache(ResSecs+nsect)) == - 1 )
    {puts("\nLinkClu: FAT read error"); return -1;}
  if ( (b2=LoadCache(ResSecs+nsect+FatSize)) == - 1 )
    {puts("\nLinkClu: FAT read error"); return -1;}

  offset=(int)(fat_byte_no%SECTOR_SIZE);
  if (offset==(SECTOR_SIZE-1))
    {
    (*(Cache+b1)).Status=2;
    cc=(unsigned char far *)((*(Cache+b1)).M+(SECTOR_SIZE-1));
    if (CluNo&1) *cc=(unsigned char)(NeXT>>4);
    else {*cc&=0xf0; *cc|=(unsigned char)(NeXT>>8);}
    if ( (b_next=LoadCache(ResSecs+nsect+1)) == - 1 )
      {puts("\nLinkClu: FAT read error"); return -1;}
    (*(Cache+b_next)).Status=2;
    cc=(unsigned char far *)((*(Cache+b_next)).M);
    if (CluNo&1) {*cc&=0x0f; *cc|=((unsigned char)(NeXT>>8))<<4;}
    else *cc=(unsigned char)(NeXT&0xff);

      {
      (*(Cache+b2)).Status=2;
      cc=(unsigned char far *)((*(Cache+b2)).M+(SECTOR_SIZE-1));
      if (CluNo&1) *cc=(unsigned char)(NeXT>>4);
      else {*cc&=0xf0; *cc|=(unsigned char)(NeXT>>8);}
      if ( (b_next=LoadCache(ResSecs+nsect+1+FatSize)) == - 1 )
        {puts("\nLinkClu: FAT read error"); return -1;}
      (*(Cache+b_next)).Status=2;
      cc=(unsigned char far *)((*(Cache+b_next)).M);
      if (CluNo&1) {*cc&=0x0f; *cc|=((unsigned char)(NeXT>>8))<<4;}
      else *cc=(unsigned char)(NeXT&0xff);
      }
    }
  else
    {
    (*(Cache+b1)).Status=2;
    ii=(unsigned int far *)((*(Cache+b1)).M+offset);
    if (CluNo&1) {*ii&=0xf; *ii|=((unsigned int)NeXT)<<4;}
    else {*ii&=0xf000; *ii|=((unsigned int)NeXT)&0xfff;}

      {
      (*(Cache+b2)).Status=2;
      ii=(unsigned int far *)((*(Cache+b2)).M+offset);
      if (CluNo&1) {*ii&=0xf; *ii|=((unsigned int)NeXT)<<4;}
      else {*ii&=0xf000; *ii|=((unsigned int)NeXT)&0xfff;}
      }
    }
break;
case FAT16:
  /* ®¯à¥¤¥«ï¥¬ ­®¬¥à ¡ ©â  ¢ â ¡«¨æ¥ FAT-16 */
  fat_byte_no=((long)CluNo)*2;
  /* ®¯à¥¤¥«ï¥¬ ­®¬¥à á¥ªâ®à  FAT */
  nsect=(int)(fat_byte_no/SECTOR_SIZE);
  /* nsect-®â­®á¨â¥«ì­ë© ­®¬¥à á¥ªâ®à  FAT. 0 - ¯¥à¢ë© á¥ªâ®à FAT */
  if (nsect>=FatSize) {puts("LinkClu: Invalid FAT's computing"); return -1;}
  if ( (b1=LoadCache(ResSecs+nsect)) == - 1 )
    {puts("\nLinkClu: FAT read error"); return -1;}
    if ( (b2=LoadCache(ResSecs+nsect+FatSize)) == - 1 )
      {puts("\nLinkClu: FAT read error"); return -1;}

  offset=(int)(fat_byte_no%SECTOR_SIZE);
  (*(Cache+b1)).Status=2;
  *(int far *)((*(Cache+b1)).M+offset)=((unsigned int)NeXT);

    {
    (*(Cache+b2)).Status=2;
    *(int far *)((*(Cache+b2)).M+offset)=((unsigned int)NeXT);
    }
break;
case FAT32:    break;
case FAT32LBA:
  /* ®¯à¥¤¥«ï¥¬ ­®¬¥à ¡ ©â  ¢ â ¡«¨æ¥ FAT-32LBA */
  fat_byte_no=((long)CluNo)*4;
  /* ®¯à¥¤¥«ï¥¬ ­®¬¥à á¥ªâ®à  FAT */
  nsect=(int)(fat_byte_no/SECTOR_SIZE);
  /* nsect-®â­®á¨â¥«ì­ë© ­®¬¥à á¥ªâ®à  FAT. 0 - ¯¥à¢ë© á¥ªâ®à FAT */
  if (nsect>=FatSize) {puts("LinkClu: Invalid FAT's computing"); return -1;}
  if ( (b1=LoadCache(ResSecs+nsect)) == - 1 )
    {puts("\nLinkClu: FAT read error"); return -1;}
    if ( (b2=LoadCache(ResSecs+nsect+FatSize)) == - 1 )
      {puts("\nLinkClu: FAT read error"); return -1;}

  offset=(int)(fat_byte_no%SECTOR_SIZE);
  (*(Cache+b1)).Status=2;
  *(long far *)((*(Cache+b1)).M+offset)=NeXT;

    {
    (*(Cache+b2)).Status=2;
    *(long far *)((*(Cache+b2)).M+offset)=NeXT;
    }
break;
default: ;
}

return 0;
}
/****************************************************************************/
unsigned long GetFreeClu(void)
{int i;
/* ¯®¨áª ¯® FAT */
for(i=2;i<=MaxClusters;i++)
  {
  if (NextClu(i)==0)
    {/* ­ è«¨ ¯ãáâ®© ª« áâ¥à */
    /* § ­¨¬ ¥¬ ¥£® ¨ ¢®§¢à é ¥¬ ¥£® ­®¬¥à */
    if (LinkClu(i,0xffffffffL)!=-1) return i; else return -1;
    }
  }
/* ­¥â ¯ãáâëå ª« áâ¥à®¢ */
return -1;
}
/****************************************************************************/
unsigned long AppendClu(unsigned long CluNo)
{unsigned long i;
if (CluNo>MaxClusters)
  {puts("AppendClu: Invalid cluster number"); return -1;}

if ((i=GetFreeClu())==0xFFFFFFFFL) return 0xFFFFFFFFL;
/* ­ è«¨ ¯ãáâ®© ª« áâ¥à */
/* ä®à¬¨àã¥¬ ¯®á«¥¤­¥¥ §¢¥­® æ¥¯®çª¨ ª« áâ¥à®¢ */
if (LinkClu(CluNo,i)==-1) return 0xFFFFFFFFL; else return i;
}
/*--------------------------------------------------------------------------*/
char dos_letter(int dev)
{
if (dev<2)
  {
  return dev+'A';
  }
else
  {
  return (dev-2)/2+'C';
  }
}
/*--------------------------------------------------------------------------*/
void print_mount(void)
{

if (CurrentDevice==-1) puts("No device mount");
else
  {
  printf("Mount at %i (DOS %c:)",CurrentDevice,dos_letter(CurrentDevice));
  printf(" mode read");
  if (ReadOnly) printf(" only");
  else printf("/write");
  if (emulation_mode==1)
    printf("\n(running under MSDOS or MSDOS-emulator (f.e. Windows 95))");
  printf("\n");
  printf("FATMode=%i\n",FATMode);
  }
return;
}
/*--------------------------------------------------------------------------*/
void mount(int NewDevice)
{
int i;
unsigned char sec, head, trk;
struct MBRstru *MBR;
struct BootStru far *B;
struct BootStru32 far *B32;
char Buf [SECTOR_SIZE];

if (NewDevice==-1)
  {
  print_mount();
  return;
  }

if (CurrentDevice!=-1)
#if 0
  {
  printf("Already ");
  print_mount();
  return;
  }
#else
  {
  umount();
  }
#endif

if(Cache==NULL)
  {puts("mount: No memory for disk cache"); return;}

for(i=0;i<BUFFERS;i++)
  (*(Cache+i)).Status=0;

CurrentDevice=NewDevice;

#ifndef NODISK
if (CurrentDevice>1)
  { /* HDD */
  if ((Devices[NewDevice].sec==0)||((FATMode=Devices[NewDevice].FileSystem)==NO_FAT))
    {
    printf("mount: invalid device\n");
    CurrentDevice=-1;
    return;
    }
  head=Devices[NewDevice].head;
  sec=Devices[NewDevice].sec;
  trk=Devices[NewDevice].trk;
  if (ReadPhysSec2 (_HDD_, sec, head, trk, Buf))
    printf ("boot: partition boot sector read error\n");
  #if 1
  /* ‚ë¢®¤ ¡ãâ-á¥ªâ®à  */
  out_boot32(Buf);
    #if 0
    printf("-press any key-"); getch();
    #endif
  #endif
  #if 0
  printf("partition boot sector loaded OK\n");
  #endif
  B = (void far *) Buf;
  B32 = (void far *) Buf;
  HeadCnt=B->HeadCnt;
  TrkSecs=B->TrkSecs;
  SectorsOnCyl=HeadCnt*TrkSecs;

  #if 1
  printf("HeadCnt=%i ",HeadCnt);
  printf("TrkSecs=%i ",TrkSecs);
  printf("SectorsOnCyl=%i ",SectorsOnCyl);
  #endif

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

  #if 1
  printf("ResSecs=%lu ",ResSecs);
  #endif

  MaxSectors=Devices[NewDevice].MaxSectors+ResSecs;

  #if 1
  printf("MaxSectors=%lu ",MaxSectors);
  #endif

  }
else
  { /* FDD */
  FATMode=FAT12;
  if (NewDevice==1) if (Devices[NewDevice].sec==0)
    {
    printf("mount: invalid device b:\n");
    CurrentDevice=-1;
    return;
    }
  if (bootread(CurrentDevice,Buf))
    {
    puts("mount: Boot read error");
    CurrentDevice=-1;
    }
  B = (void *) Buf;
  if ((i=B->ResSecs)!=0) ResSecs=i; ResSecs=B->HidnSec+1;
  if ((i=B->TotSecs)!=0) MaxSectors=i;
  else MaxSectors=B->BigSect+B->HidnSec;

  #if 0
  out_boot((void far *) Buf);
  #endif

  HeadCnt=B->HeadCnt;
  TrkSecs=B->TrkSecs;
  SectorsOnCyl=HeadCnt*TrkSecs;
  }

if (CurrentDevice!=-1)
  {
  CluSize=B->ClustSiz;
  CluSizeBytes=CluSize * SECTOR_SIZE;
  if ((FATMode==FAT32)||(FATMode==FAT32LBA))
    FatSize=B32->BigFATSize;
  else
    FatSize=B->FatSize;
    /* FatCnt=B->FatCnt; */
  #if 1
  printf("CluSize=%i\n",CluSize);
  printf("FatSize=%i\n",FatSize);
  printf("FatCnt=%i\n",FatCnt);
  #endif
  RootBeg = ResSecs + FatCnt * B->FatSize;
  #if 0
  printf("\nB->SectSiz=%i ",B->SectSiz);
  #endif
  RootEnd = RootBeg + ( ( B->RootSiz * 32 ) / B->SectSiz ) - 1;
  if ((FATMode==FAT32)||(FATMode==FAT32LBA))
    {
    DataStart = B32->HidnSec+B32->ResSecs+B32->BigFATSize*2;
    RootBegClu=B32->RootBegClu;
    ResSecs=B32->ResSecs+B32->HidnSec;
    }
  else
    {
    DataStart = RootEnd+1;
    RootBegClu=0;
    }
  #if 1
  printf("DataStart=%lu RootBegClu=%lu ",DataStart,RootBegClu);
  #endif
  MaxClusters=((MaxSectors-DataStart)/CluSize+1);

  #if 1 /***/
  printf("\n***HeadCnt =%2i TrkSecs =%2i CluSize =%2i FatSize =%lu FatCnt =%1i\
  RootBeg =%lu\nRootEnd =%lu ResSecs =%lu MaxSectors =%lu MaxClusters =%lu",
    HeadCnt,TrkSecs,CluSize,FatSize,FatCnt,RootBeg,RootEnd,ResSecs,
    MaxSectors,MaxClusters);
  printf("\nboot: Cluster size = %u\n",CluSizeBytes);
  #endif /***/

  CurDirClu=0;
  puts("");
  }

#endif /* NODISK */

}

/*--------------------------------------------------------------------------*/

void umount (void)
{int i;
shutdown();

CurrentDevice=-1;

if(Cache==NULL)
  {puts("umount: No memory for disk cache"); return;}

for(i=0;i<BUFFERS;i++)
  (*(Cache+i)).Status=0;

}
/*--------------------------------------------------------------------------*/
void RestoreVec(void)
{/* ¢®ááâ ­®¢«¥­¨¥ áâ àëå ¯à¥àë¢ ­¨© */
if (OldInt!=NULL)
  {
  /* puts("vectors restored"); */
__cli__();
  memcpy(MK_FP(0,0),OldInt,256*4);
__sti__();
  }
else
  puts("vectors not restored");
}
/*--------------------------------------------------------------------------*/
void init (void)
{
int i;
int CurPar;
struct MemBlk far * MB;

#if CODETABLE==_RUSSIAN_
char RussianUpper [] = "”ˆ‘‚“€˜Ž‹„œ’™‡‰Š›…ƒŒ–—Ÿ";
char RussianLower [] = "ä¨á¢ã ¯àè®«¤ìâé§©ªë¥£¬æç­ï";
#endif

#ifdef PRINTF_DEBUG
char far *far_local_init = "far_local_init";
char far *far_local_uninit;
char *near_local_init = "near_local_init";
char *near_local_uninit;
char local_array_init [] = "local_array_init";
char local_array_uninit [100];
#endif

KbdStatus=MK_FP(0x40, 0x17);

errno=0;

for (i=0; i<OPEN_MAX; i++) FCBS[i].Name[0]=0;

strcpy(FCBS[0].Name,"stdin");
FCBS[0].File=0;
FCBS[0].flag=O_RDONLY;
FCBS[0].Size=0;
FCBS[0].DirInode=-1;

strcpy(FCBS[1].Name,"stdout");
FCBS[0].File=0;
FCBS[0].flag=O_WRONLY;
FCBS[0].Size=0;
FCBS[0].DirInode=-1;

strcpy(FCBS[2].Name,"stderr");
FCBS[0].File=0;
FCBS[0].flag=O_WRONLY;
FCBS[0].Size=0;
FCBS[0].DirInode=-1;

stdin =&FCBS[0];
stdout=&FCBS[1];
stderr=&FCBS[2];

/* clrscr(); */
#ifdef PRINTF_DEBUG
far_global_uninit="far_global_uninit";
far_local_uninit="far_local_uninit";
near_global_uninit="near_global_uninit";
near_local_uninit="near_local_uninit";
strcpy(local_array_uninit,"local_array_uninit");
strcpy(global_array_uninit,"global_array_uninit");

#if 0
printf("literal");
printf("local far init"); printf(far_local_init);
printf("local far uninit"); printf(far_local_uninit);
printf("global far init"); printf(far_global_init);
printf("global far uninit"); printf(far_global_uninit);
#endif

printf("\nPRINTF:\n\nLiteral=");
printf("local  far str initialized   %%Fs %Fs\n", far_local_init);
printf("local  far str uninitialized %%Fs %Fs\n", far_local_uninit);
printf("global far str initialized   %%Fs %Fs\n",far_global_init);
printf("global far str uninitialized %%Fs %Fs\n",far_global_uninit);
printf("local  far str initialized   %%s   %s\n", far_local_init);
printf("local  far str uninitialized %%s   %s\n", far_local_uninit);
printf("global far str initialized   %%s   %s\n",far_global_init);
printf("global far str uninitialized %%s   %s\n",far_global_uninit);

printf("local  near str initialized   %%s   %s\n", near_local_init);
printf("local  near str uninitialized %%s   %s\n", near_local_uninit);
printf("global near str initialized   %%s   %s\n",near_global_init);
printf("global near str uninitialized %%s   %s\n",near_global_uninit);

printf("global array initialized %%s   %s\n",global_array_init);
printf("global array uninitialized %%s   %s\n",global_array_uninit);
printf("local array initialized %%s   %s\n",local_array_init);
printf("local array uninitialized %%s   %s\n",local_array_uninit);

printf("Printf 0 = %i\n",0);
printf("Printf 1 = %i\n",1);
printf("Press any key\n");
getch();
#endif

i = stackend();
MemTop = (*((unsigned int far *)MK_FP(0,0x413)))<<6;
printf("MemTop=%04X\n",MemTop);

/* printf("CurrentDevice=%02X\n",CurrentDevice); */
printf("Kernel begin %04X:0000 - stackend %04X:0000, total %i K\n",
kernel_begin(),i,(i-kernel_begin())/64);

Fin = MK_FP (i,0);

MB=Fin;
(*MB).Type='Z';
(*MB).OwnerPar=0;
CurPar = FP_SEG(Fin) + (FP_OFF(Fin)>>4);
(*MB).SizePar=MemTop-CurPar;

/* § ¯®¬¨­ ­¨¥ ¢á¥å ¢¥ªâ®à®¢ ¯à¥àë¢ ­¨© */

if ((OldInt=malloc(256*4))==NULL) puts("No mem for old interrupt table");

if (OldInt!=NULL)
  {
  __cli__();
  memcpy(OldInt,MK_FP(0,0),256*4);
  __sti__();
  #ifdef DEBUG
  puts("\nInterrupt vectors saved");
  #endif
  /* printf("Orig Int5=%04X:%04X\n", *(int far *)5*4,*(int far *)(5*4+2)); */
  }
else puts("init(): No mem");

setvect(0x0,Int0);

setvect(0x3,Int3);

#ifdef CHANGE_INTS
initv();
setvect(0x18,done);
setvect(0x19,CtrlAltDel);
setvect(0x1,Int1);
setvect(0x2,Int2);
setvect(0x4,Int4);
setvect(0x6,Int6);
setvect(0x7,Int7);
setvect(0x75,Int75);
#endif

/* #ifdef CHANGE_INT5 */
/* setvect(0x5,Int5); */
/* setvect(5, (void interrupt(*)(void))MK_FP(_Begin,FP_OFF(Int5dump))); */
/* puts("Int 5 hooked"); */
/* #endif */

curAddr=MK_FP(VIDEO_SEG,0);

/* ‘¬®âà¨ ª®¬¬¥­â à¨© ã ®¯¨á ­¨ï ¯à®â®â¨¯  äã­ªæ¨¨ SysCall1 (íâ® ¢ëè¥) */
/* setvect(INT_NO, (void interrupt(*)(void))MK_FP(_Begin,FP_OFF(SysCall1))); */
setvect(INT_NO, SysCall2);

#ifdef DEBUG
printf("before MSHerc\n");
#endif

#if defined(_HERCULES_) && defined(_MSHERC_)
if(!mshercinit())
  printf("\nHercules Resident Video Support Routines. Version 1.10+Proolix");
else
  printf("\nHercules Video Card not present.");
#endif

OldTimerVec=getvect(0x8);
/* setvect(0x8,Int8); */
/* setvect(0x85,Int8); */

#if CODETABLE==_RUSSIAN_
/* russification module initialization */

for (i=0;i<256;i++) Russian[i]=i;

for (i='A';i<='Z';i++) Russian[i]=RussianUpper[i-'A'];
for (i='a';i<='z';i++) Russian[i]=RussianLower[i-'a'];
Russian['[']='å';
Russian['{']='•';
Russian[']']='ê';
Russian['}']='š';
Russian[';']='¦';
Russian[':']='†';
Russian['\'']='í';
Russian['"']='';
Russian[',']='¡';
Russian['<']='';
Russian['.']='î';
Russian['>']='ž';
#endif

if ((hEnv=malloc(ARG_MAX))==NULL) puts("init: No mem for argv");
if ((pEnv=malloc(MAX_ENV*4))==NULL) puts("init: No mem for main env.");

for (i=0; i<MAX_ENV; i++) *(pEnv+i)=NULL;

*(pEnv+ 0)="PATH=:/bin";
#if 0
*(pEnv+ 1)="HOME=/";
*(pEnv+ 2)="LC_COLLATE=C";
*(pEnv+ 3)="LC_CTYPE=C";
*(pEnv+ 4)="LC_MONETARY=C";
*(pEnv+ 5)="LC_NUMERIC=C";
*(pEnv+ 6)="LC_TIME=C";
*(pEnv+ 7)="LOGNAME=root";
*(pEnv+ 8)="LANG=C";
*(pEnv+ 9)="TERM=tty";
*(pEnv+10)="TZ=2";
#endif

#if 0
/* For UNISCR.COM by L.Bunich: */
allow_change_int [0x1F]=1;
allow_change_int [0x43]=1;
#endif

#if 0
/* For ne2000.com: */
allow_change_int [0xD]=1;
allow_change_int [0x60]=1;
#endif

Cache=malloc(BUFFERS*sizeof(struct BufPool));

for (i=0;i<PROC_MAX;i++)
  {
  process_table[i].pid=0;
  process_table[i].status=0;
  process_table[i].mem0=0;
  }

if ((i=CurrentDevice)>1) ReadOnly = HDD_READ_ONLY;
else ReadOnly = 0;

  {unsigned int seg;
  seg=FP_SEG((unsigned long)getvect(0x21));
  printf("int 21h seg=%04X\n",seg);
  if (seg!=0xF000)
    {emulation_mode=1; i=2; /* ReadOnly=1; */ }
  else
    {emulation_mode=0;}
  }

CurrentDevice=-1;
mount(i);
}
/****************************************************************************/
void test_devices(void)
{unsigned int i;
struct MBRstru *MBR;
char MBRBuf [SECTOR_SIZE];

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
#if 1
printf("disk drives number = %i\n\n",i);
#endif
if (i) Devices[0].sec=-1;
if (i>1) Devices[1].sec=-1;

if (bootread0 (_HDD_, MBRBuf))
  printf ("test_devices: MBR read error\n");

#if 1
outMBR(MBRBuf);
#endif

#if 0
printf("# head sec trk ResSecs MaxSectors\n");
#endif

process_partition_table (MBRBuf);

}