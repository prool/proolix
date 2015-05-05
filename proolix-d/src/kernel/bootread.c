#include <prool.h>

#include <conf.c>
#include "kernel.h"

/****************************************************************************/
int bootread (int Dev, void far *Buf)

{int i, err;
char InternalBuffer [SECTOR_SIZE];
for (i=0; i<RETRYES; i++)
  {
  err = bootread0 (Dev,InternalBuffer);
  if (!err)
    {
    memcpy(Buf,InternalBuffer,SECTOR_SIZE);
    return 0;
    }
  }
return -1;
}
