#include <prool.h>
#include <dos.h>

#include <conf.c>

#include "kernel.h"

/* #define DEBUG */

/* NextClu(), NextClu2() - next cluster number compute and load to Cache) */
/* NextClu() - kernel's internal module */
/* NextClu2() - boot manager internal module */

/* Согласовывать NextClu() с NextClu2() ! */

unsigned long NextClu (unsigned long CluNo) /* -1 for eof or error */
{
unsigned long j;
unsigned long nsect;
unsigned int offset, b;
unsigned long i;

if ((CluNo>MaxClusters)||(CluNo==0))
  return -1;

switch (FATMode)
{
case FAT12:
  { /* FAT 12 */
  /* определяем номер байта в таблице FAT-12 */
  j=(CluNo*3)/2;
  /* определяем номер сектора FAT */
  nsect=(unsigned int)(j/SECTOR_SIZE);
  #ifdef DEBUG
  printf("nsect=%i ",nsect);
  #endif
  /* nsect-относительный номер сектора FAT. 0 - первый сектор FAT */
  if (nsect>=FatSize) {puts("NextClu: Invalid FAT's computing"); return -1;}
  if ( (b=LoadCache(ResSecs+nsect)) == -1U )
    {puts("\nNextClu: FAT read error"); return -1;}

  offset=(unsigned int)(j%SECTOR_SIZE);
  #ifdef DEBUG
  printf("offset=%i ",offset);
  #endif
  if (offset==(SECTOR_SIZE-1))
    {unsigned char c;
    c=*(unsigned char far *)((*(Cache+b)).M+(SECTOR_SIZE-1));
    if ( (b=LoadCache(ResSecs+nsect+1)) == -1U )
      {puts("\nNextClu: FAT read error"); return -1;}
    #ifdef DEBUG
    printf("c=%04X ",c);
    #endif
    i=((unsigned int)c)|
    (((unsigned int)(*(unsigned char far *)((*(Cache+b)).M)))<<8);
    }
  else
    i=*(int far *)((*(Cache+b)).M+offset);
  #ifdef DEBUG
  printf("word=%04X ",i);
  #endif
  if (CluNo & 1)
    {
    #ifdef DEBUG
    putch('n');
    #endif
    i>>=4;
    }
  else
    {
    #ifdef DEBUG
    putch('c');
    #endif
    i&=0xfff;
    }
  if (i>0xff0) return -1;
  }
break;
case FAT16:
  { /* FAT 16 */
  /* определяем номер байта в таблице FAT-16 */
  j=((long)CluNo)*2;
  /* определяем номер сектора FAT */
  nsect=(unsigned int)(j/SECTOR_SIZE);
  /* nsect-относительный номер сектора FAT. 0 - первый сектор FAT */
  if (nsect>=FatSize) {puts("NextClu: Invalid FAT's computing"); return -1;}
  if ( (b=LoadCache(ResSecs+nsect)) == -1U )
    {puts("\nNextClu: FAT read error"); return -1;}

  offset=(unsigned int)(j%SECTOR_SIZE);
  i=*(int far *)((*(Cache+b)).M+offset);
  if (i>0xfff0) return -1;
  }
break;
case FAT32:
case FAT32LBA:
  {
  /* определяем номер байта в таблице FAT-32 */
  j=CluNo*4;
  /* определяем номер сектора FAT */
  nsect=j/SECTOR_SIZE;
  /* nsect-относительный номер сектора FAT. 0 - первый сектор FAT */
  if (nsect>=FatSize) {puts("NextClu: Invalid FAT's computing"); return -1;}
  if ( (b=LoadCache(ResSecs+nsect)) == -1U )
    {puts("\nNextClu: FAT read error"); return -1;}

  offset=(unsigned int)(j%SECTOR_SIZE);
  i=*(long far *)((*(Cache+b)).M+offset);
  if (i>0xfffffff0L) return -1;
  i=i&0x3FFFFFFFL;
  }
break;
default: ;
}

#if 0
printf("%i->%i ",CluNo,i);
#endif

return i;
}