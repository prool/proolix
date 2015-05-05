#include <prool.h>
#include <stdio.h>

#include <conf.c>

#include "kernel.h"
/****************************************************************************/
/* #define DEBUG */
/****************************************************************************/
unsigned long SecForClu (unsigned long CluNo)
{unsigned long l;
/*
Sector=ResSecs + FatCnt*FatSize + (RootSiz*32)/SectSiz + ((CluNo-2)*ClustSiz)
       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
       DataStart
*/
if ((CluNo>MaxClusters)||(CluNo<2))
  {
  #if 0
  printf("SecForClu: Invalid cluster number. CluNo=%u, MaxClusters=%u",
  CluNo,MaxClusters);
  #endif
  return -1;
  }
l=DataStart + ( (CluNo-2) * CluSize );
#ifdef DEBUG
printf("sec4clu: return %li ",l);
#endif
return l;
}