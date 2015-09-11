#include <stdio.h>
#include "..\include\struct.h"

void out_os (unsigned char c);

void outMBR (char far * buf)
{
int i;
struct MBRstru far * MBR;

MBR = (void far * ) buf;

printf("  System         ----Begin--- ----End----- Preceding sec  Total sec  Size,Mb\n");
printf("                 head sec cyl head sec cyl\n");
/*     "A FAT 12  iiii  ii iiii iiii  ii iiii iiiiiiiiii    iiiiiiiiii\n" */
for (i=0;i<4;i++)
  {unsigned int j;
  if (MBR->Partition[i].indicator==0) printf(" "); else printf("A");
  printf(" ");
  out_os(MBR->Partition[i].system_indicator);
  printf("(%02X) ",MBR->Partition[i].system_indicator);
  printf(" %4i  ",MBR->Partition[i].begin_head);
  printf("%2i ",(j=MBR->Partition[i].begin_sec) & 0x3F);
  j = (j & 0xC0)<<2;
  printf("%4i ",MBR->Partition[i].begin_cyl | j);

  printf("%4i  ",MBR->Partition[i].end_head);
  printf("%2i ",(j=MBR->Partition[i].end_sec) & 0x3F);
  j = (j & 0xC0)<<2;
  printf("%4i ",MBR->Partition[i].end_cyl | j);

  printf("%10lu    ",MBR->Partition[i].preceding_sec);
  printf("%10lu ",MBR->Partition[i].total_sec);
  printf("%4i\n",(MBR->Partition[i].total_sec)/2048);
  }
}

