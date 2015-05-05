#include <prool.h>
#include <proto.h>
#include <dos.h>
#include <dirent.h>

#include <conf.c>

/* #define DEBUG */

extern int CluSize;

int CluRead2 (int drive, unsigned int CluNo, void far * buffer)
{unsigned int i;

#ifdef DEBUG
printf("\nCluRead=%u ",CluNo);
#endif

i=absread (TR(drive), CluSize,SecForClu2(CluNo), buffer);
return i;
}