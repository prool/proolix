#include <prool.h>

#include <conf.c>
#include "kernel.h"

/* #define DEBUG */

void out_boot32(void far *buf)

{int i;
unsigned long DiskSize;
unsigned long TrueSectors;
struct BootStru32 far *b;

b=buf;

printf("Jump command %02X %02X %02X  \n",(*b).Jmp[0],(*b).Jmp[1],(*b).Jmp[2]);
printf("OEM name     %-8s\n\
OEM name     ",(*b).OEM);

for(i=0;i<8;) printf("%02X ",(*b).OEM[i++]);

printf("\nSector size                      %4i bytes Cluster size                %1i sect\n",(*b).SectSiz,(*b).ClustSiz);
printf("Reserved sectors  %2i       FAT counter                 %1i\n",(*b).ResSecs,(*b).FatCnt);
printf("Root directory entries           %4i       Total sectors         %7u\n",(*b).RootSiz,(*b).TotSecs);
printf("Media descr                        %02X       FAT size                %5i sect\n",(*b).Media,(*b).FatSize);
printf("Track size                         %2i sec   Heads                     %3i\n",(*b).TrkSecs,(*b).HeadCnt);
printf("Hidden sectors                %7li       Big Number Of Sectors %7li\n",(*b).HidnSec,(*b).BigSect);

printf("BigFATSize %lu ",(*b).BigFATSize);
printf("ExtendedFlags %02X ",(*b).ExtendedFlags);
printf("FilesystemVer %i ",(*b).FilesystemVer);
printf("RootBegClu %lu ",(*b).RootBegClu);
printf("AuxSector %i ",(*b).AuxSector);
printf("CopySector %i ",(*b).CopySector);

#if 0
printf("\nReserved          ");
for(i=0;i<12;)putchp((*b).Reserved[i++]);
printf("\n                        ");
for(i=0;i<12;)printf("%02X ",(*b).Reserved[i++]);

printf("\nString          ");
for(i=0;i<8;)putchp((*b).String[i++]);
printf("\n                        ");
for(i=0;i<8;)printf("%02X ",(*b).String[i++]);
#endif

}