#include <prool.h>
#include <dos.h>

#include <conf.c>

/* NextClu(), NextClu2() - next cluster number compute and load to Cache) */
/* NextClu() - kernel's internal module */
/* NextClu2() - boot manager internal module */

/* Согласовывать NextClu() с NextClu2() ! */

extern unsigned int MaxClusters;
extern int FATMode;
extern char huge * FAT;

unsigned int NextClu2 (unsigned int CluNo) /* -1 for eof or error */
{ /* for FAT12 */
unsigned int i;

if ((CluNo>MaxClusters)||(CluNo==0))
  {
  printf("NextClu: Invalid cluster number. CluNo=%u, MaxClusters=%u",
  CluNo, MaxClusters);
  return -1;
  }

if (FATMode==FAT12)
  { /* FAT 12 */
  i=*(unsigned int huge *)(FAT+ (((unsigned long)CluNo) * 3)/2);
  if (CluNo & 1) i>>=4; else i&=0xfff;
  if (i>0xff0) return -1;
  }
else
  { /* FAT 16 */
  i=*(unsigned int far *)(FAT+ ((unsigned long)CluNo) * 2);
  if (i>0xfff0) return -1;
  }

return i;
}