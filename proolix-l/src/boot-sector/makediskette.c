/*
OS Proolix-l
Utility for make proolix-l diskette image for VirtualBox
27.08.2014
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define S1 "This program makes Proolix-l bootable diskette\n\nusage: makediskette emptydiskette.img bootsector.bin\n\
\n"
#define S1a "emptydiskette.img + bootsector.bin -> emptydiskette.img\n\n"
#define S2 "can't open %s\n"
#define S3 "can't read %s\n"
#define S4 "can't write %s\n"
#define S5 "can't close %s\n"

int main (int argc, char *argv[])
{int i, h1, h2, h3, h4;
unsigned char c[512], c2[512], c3[512];

#if 0
printf ("argc=%i\n",argc);
for (i=0;i<argc; i++) printf ("argv[%i]=%s\n",i,argv[i]);
#endif

if (argc!=3)
  {
  printf (S1);
  printf (S1a);
  return 1;
  }

/* read old (original) boot sector */  
if ((h1=open(argv[1],0))<=0) {printf(S2,argv[1]); return 2; }
if (read(h1,c,512)!=512) {printf(S3,argv[1]); return 3; }
close(h1);

#if 0
for (i=0;i<512;i++) printf ("%02X",c[i]);
printf("\n");
#endif

/* read new boot sector */
if ((h2=open(argv[2],0))<=0) {printf(S2,argv[2]); return 4; }
i=read(h2,c2,512);
if (i==-1) {printf(S3,argv[2]); return 5; }
//if (i>512) {printf("New boot sector too long!!!!!\n"); return 50;}

// patch
for (i=0;i<3+8;i++)c[i]=c2[i]; /* JMP and OEM name */
for (i=0x26; i<510; i++) c[i]=c2[i]; /* bootloader */
c[510]=0x55;
c[511]=0xaa;

/* write new boot sect */
if ((h3=open(argv[1],O_RDWR/*S_IRUSR*/))<=0) {printf(S2,argv[1]); return 5; }
if (write(h3,c,512)!=512) {printf(S4,argv[1]); return 6; }

#if 0
// write stage 2

if ((h4=open(argv[3],0))<=0) {printf(S2,argv[3]); return 5; }
while(1)
	{
	if (read(h4,c2,512)<=0) {printf("EOF\n"); break; }
	if (write(h3,c2,512)!=512) {printf(S4,argv[1]); return 6; }
	}
#endif

if (close(h3)==-1) {printf(S5,argv[1]); return 7; }
return 0;
}
