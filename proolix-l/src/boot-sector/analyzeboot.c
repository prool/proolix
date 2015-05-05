/*
OS Proolix-l
Boot analyze utulity
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define S1 "usage: analyzeboot source-bootfile\n"
#define S2 "can't open %s\n"
#define S3 "can't read %s\n"

struct BootStru
  {
  unsigned char Jmp [3];
  unsigned char OEM [8];
  short SectSiz;
  char ClustSiz;
  unsigned short  ResSecs;
  char FatCnt;
  short  RootSiz;
  unsigned short  TotSecs;
  unsigned char Media;
  short  FatSize;
  short  TrkSecs;
  short  HeadCnt;
  unsigned int HidnSec;
  unsigned int BigSect;
  unsigned char DriveNo;
  unsigned char Thing;
  unsigned char BootSign;
  short  SerialNo [2];
  unsigned char VolLbl [11];
  unsigned char FileSysId [8];
  /*
  short  FATaddr;
  short  StartClu;
  short  RootSize;
  */
  } __attribute__((packed));

void out_boot(void *buf)

{int i;
unsigned long DiskSize;
unsigned long TrueSectors;
struct BootStru *b;
b=buf;
printf("Jump command %02X %02X %02X  \n",(*b).Jmp[0],(*b).Jmp[1],(*b).Jmp[2]);
printf("OEM name     %-8s\n\
OEM name     ",(*b).OEM);

for(i=0;i<8;) printf("%02X ",(*b).OEM[i++]);

#if 1
printf("\nSector size                       %i bytes Cluster size                %1i sect\n",b->SectSiz,(*b).ClustSiz);
printf("Reserved sectors (before 1st FAT)  %2i       FAT counter                 %1i\n",(*b).ResSecs,(*b).FatCnt);
printf("Root directory entries           %4i       Total sectors         %7u\n",(*b).RootSiz,(*b).TotSecs);
printf("Media descr                        %02X       FAT size                %5i sect\n",(*b).Media,(*b).FatSize);
printf("Track size                         %2i sec   Heads                      %2i\n",(*b).TrkSecs,(*b).HeadCnt);
printf("Hidden sectors                %7li       Big Number Of Sectors %7li\n",(*b).HidnSec,(*b).BigSect);
printf("Physical Drive No                  %02X       Extended Boot Signature    %02X\n",(*b).DriveNo,(*b).BootSign);
printf("Volume Serial No %04X-%04X",
(*b).SerialNo[1],
(*b).SerialNo[0]);
#endif

printf("\nVolume Label (in boot)  ");
for(i=0;i<11;)putchar((*b).VolLbl[i++]);
printf("\n                        ");
for(i=0;i<11;)printf("%02X ",(*b).VolLbl[i++]);
printf("\nFile system Id          ");
for(i=0;i<8;)putchar((*b).FileSysId[i++]);
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

int main (int argc, char *argv[])
{int i, h1;
unsigned char c[512];

#if 0
printf ("argc=%i\n",argc);
for (i=0;i<argc; i++) printf ("argv[%i]=%s\n",i,argv[i]);
#endif

if (argc!=2)
  {
  printf (S1);
  return 1;
  }

/* read original boot sector */  
if ((h1=open(argv[1],0))<=0) {printf(S2,argv[1]); return 2; }
if (read(h1,c,512)!=512) {printf(S3,argv[1]); return 3; }

out_boot(c);

#if 0
for (i=0;i<512;i++) printf ("%02X",c[i]);
printf("\n");
#endif

return 0;
}
