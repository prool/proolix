#include <prool.h>

#include <conf.c>
#include "kernel.h"
/****************************************************************************/
char *Int13error [] = {
"\000No errors",
"\001Bad controller command",
"\002Bad adress mark",
"\003Write protect",
"\004Sector ID bad or not found",
"\005Reset controller error (AT)",
"\010DMA fault",
"\011DMA overload (over 64K frontiere)",
"\013Bad track flag (AT)",
"\020CRC error",
"\021ECC error fixed (AT)",
"\040Controller fault",
"\100Seek error, track not found",
"\200Timeout",
"\273Undefined error (AT)",
"\377Sense error (AT)"
};
/****************************************************************************/
int Int13perror(unsigned err)
{int i;
for (i=0;;i++)
  {
  if (Int13error[i][0]==err) return printf(Int13error[i]+1);
  if (Int13error[i][0]==(char)0xff) break;
  }
return printf("Illegal error code");
}
/****************************************************************************/
