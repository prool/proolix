#include <prool.h>
#include <proto.h>
#include <dos.h>
#include <dirent.h>

#include <conf.c>

/* #define DEBUG */

extern int CluSize;

int CluRead (int drive, unsigned long CluNo, void far * buffer)
{unsigned int i;

#ifdef DEBUG
printf("\nCluRead=%u ",CluNo);
#endif

i=absread (TR(drive), CluSize,SecForClu(CluNo), buffer);
return i;
}