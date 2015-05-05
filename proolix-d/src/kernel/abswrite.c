#include <prool.h>

#include <conf.c>
#include "kernel.h"

extern unsigned char NearBuffer [];
int Int13perror(unsigned err);
/****************************************************************************/
/* #define DEBUG */
/****************************************************************************/
extern unsigned char NearBuffer [SECTOR_SIZE]; /* defined in kernel.c */
/****************************************************************************/
int secwrite (int drive, unsigned long AbsSec, char *Buffer)
{/* Write absolute sectors
Input:
drive (for int 13h Fn=2)
abs sec number
buffer

Output: return!=0 if error
*/
int Track, SecNoOnCyl, i;
char Head, SecOnTrk;

Track=(int)(AbsSec/SectorsOnCyl); /* SectorsOnCyl=HeadCnt*TrkSecs, Track == Cyl */
SecNoOnCyl=(int)(AbsSec%SectorsOnCyl);
Head=SecNoOnCyl/TrkSecs;
SecOnTrk=SecNoOnCyl%TrkSecs+1;
       /*
       2 bytes are combined to a word similar to INT 13:

       ³7³6³5³4³3³2³1³0³ 1st byte  (sector)
        ³ ³ ÀÄÁÄÁÄÁÄÁÄÁÄÄ Sector offset within cylinder
        ÀÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄ High order bits of cylinder #

       ³7³6³5³4³3³2³1³0³ 2nd byte  (cylinder)
        ÀÄÁÄÁÄÁÄÁÄÁÄÁÄÄÄÄÄ Low order bits of cylinder #
        */
if ((i=Track &0x0300)!=0)
  {
  SecOnTrk = (SecOnTrk & 0x3F) | (int)(i>>2);
  }
return WritePhysSec(drive, SecOnTrk, Head, Track, Buffer);
}
/****************************************************************************/
int abswrite (int drive, int nsects, unsigned long lsect, void far *FarBuffer)
{
int i, j, k;
unsigned err;

if (ReadOnly)
  {
  puts("Drive has mounted read only. Write not allowed!");
  return -1;
  }

for (i=0;i<nsects;i++,lsect++)
  {
  if (lsect>=MaxSectors)
    {
    puts("absread: lsect >=MaxSectors");
    return -1;
    }
  #if 1
  for (k=0;k<SECTOR_SIZE;k++)
    {
    NearBuffer[k]=*((char far *)FarBuffer)++;
    /* putch('.'); */
    }
  #else
  memcpy(NearBuffer, FarBuffer, SECTOR_SIZE);
  #endif

  while(1)
    {
    err=1;
    for (j=0;j<RETRYES;j++)
      if ((err=secwrite (drive, lsect, NearBuffer))==0) break;
    if (err)
      {err>>=8;
      printf("absread: error %2X in sec %i: ",err,lsect);
      Int13perror(err);
      printf(" Ignore (i), retry (r) or abort (a) ? ");
      j=getchar();
      puts("");
      if (j=='r') continue;
      else if (j=='a') return -1;
      else /* ignore */ break;
      }
    else
      break;
    }
  /* FarBuffer = (char far *) FarBuffer + SECTOR_SIZE; */
  }
return 0;
}
/****************************************************************************/
