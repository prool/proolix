#include <prool.h>
#include <stdio.h>

#include <conf.c>

#include "kernel.h"
/****************************************************************************/
/* #define DEBUG */
/****************************************************************************/
unsigned long SecForClu2 (unsigned int CluNo)
{unsigned long l;
/*
Sector=ResSecs + FatCnt*FatSize + (RootSiz*32)/SectSiz + ((CluNo-2)*ClustSiz)
       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
       DataStart
*/
#if 0
if ((CluNo>MaxClusters)||(CluNo<2))
  {
  printf("SecForClu2: Invalid cluster number. CluNo=%i, MaxClusters=%i",
  CluNo,MaxClusters);
  return -1;
  }
#endif
l=DataStart + ( ((unsigned long)CluNo-2) * CluSize );
#ifdef DEBUG
printf("sec4clu: return %li ",l);
#endif
return l;
}