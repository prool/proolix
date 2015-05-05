#include <prool.h>

#include <conf.c>
#include "kernel.h"
/****************************************************************************/
/* #define DEBUG */
char InternalBuffer [SECTOR_SIZE];
/****************************************************************************/
int ReadPhysSec2 (unsigned char drive, unsigned char sec, unsigned char head,
                  unsigned char trk /* or cyl */,
                  void far *Buf)

{int i, err;

#ifdef DEBUG
printf("\nReadPhysSec2. drive=%i. sec=%i. head=%i. trk=%i.\n",drive,sec,head,
  trk);
#endif

for (i=0; i<RETRYES; i++)
  {
  err = ReadPhysSec (drive, sec, head, trk, InternalBuffer);
  if (!err)
    {
    #ifdef DEBUG
    printf("ReadPhysSec2: boot sign=%s\n",InternalBuffer+3);
    #endif

    memcpy(Buf,InternalBuffer,SECTOR_SIZE);

    #ifdef DEBUG
    printf("ReadPhysSec2: boot sign=%Fs\n",(char far *)Buf+3);
    #endif

    return 0;
    }
  }
return -1;
}
