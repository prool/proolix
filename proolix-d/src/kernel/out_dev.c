#include <conf.c>
#include <stdio.h>
#include "kernel.h"

void out_os (int c);

void out_devices (void)
{int i;

printf("\nDev  Head Sec Trk/cyl Partition");
printf("\n-------------------------------");
for (i=0;i<LASTDRIVE;i++)
  {
  if (Devices[i].sec)
    {
    printf("\n%1i ",i);
    printf("   ");
    switch (i)
      {
      case 0:
      case 1:  printf("floppy drive"); break;
      default: /* printf("%i ",Devices[i].FileSystem); */ /* FAT12, FAT16 */
               printf("%4i ",Devices[i].head);
               printf("%3i ",Devices[i].sec);
               printf("%3i     ",Devices[i].trk);
               out_os(Devices[i].system_indicator);
               printf(" (%02X)",Devices[i].system_indicator);
               if (Devices[i].dos_disk!=' ')
                 {
                 printf(" %c:",Devices[i].dos_disk);
                 switch (Devices[i].FileSystem)
                   {
                   case FAT12:    printf(" FAT-12"); break;
                   case FAT16:    printf(" FAT-16"); break;
                   case FAT32:    printf(" FAT-32"); break;
                   case FAT32LBA: printf(" FAT32L"); break;
                   default:       printf(" no FAT");
                   }
                 }
               /* printf("%6lu ",Devices[i].ResSecs); */
               /* printf("%6lu",Devices[i].MaxSectors); */
      }
    }
  }
printf("\n");
}


