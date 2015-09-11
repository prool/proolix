#include <stdio.h>
#ifndef MSDOS
#include <dos.h>
#endif

#ifdef MSDOS
int absread (int drive, int nsects, unsigned long lsect, void far *FarBuffer);

struct Partition_stru
  {
  unsigned char   indicator;
        /*
        00  - non-bootable partition
        80  - bootable partition (one partition only)
        */
  unsigned char   begin_head;
  unsigned char   begin_sec; /* and 2 high bits of cylinder # */
  unsigned char   begin_cyl; /* low order bits of cylinder # */
        /*
        2 bytes are combined to a word similar to INT 13:

        ³7³6³5³4³3³2³1³0³ 1st byte  (sector)
         ³ ³ ÀÄÁÄÁÄÁÄÁÄÁÄÄ Sector offset within cylinder
         ÀÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄ High order bits of cylinder #

        ³7³6³5³4³3³2³1³0³ 2nd byte  (cylinder)
         ÀÄÁÄÁÄÁÄÁÄÁÄÁÄÄÄÄÄ Low order bits of cylinder #
         */
  unsigned char   system_indicator;
  unsigned char   end_head;
  unsigned char   end_sec; /* and 2 high bits of cylinder # */
  unsigned char   end_cyl; /* low order bits of cylinder # */
  unsigned long   preceding_sec;
  unsigned long   total_sec;
  };
struct MBRstru
  {
  char master_boot_loader [0x1BE];
  struct Partition_stru Partition [4];
  unsigned int Signature; /* 55AA */
  };
#else /* ifdef Proolix */
#include <struct.h>
#endif

void out_os (int c);

void outMBR (char far *buf)
{
int i;
struct MBRstru far *MBR;

MBR=(void far *)buf;

printf("System    ----Begin---- ----End-----  Preceding sec Total sec\n");
printf("          head sec  cyl head sec cyl\n");
/*     "A FAT 12  iiii  ii iiii iiii  ii iiii iiiiiiiiii    iiiiiiiiii\n" */
for (i=0;i<4;i++)
  {unsigned int j;
  if (MBR->Partition[i].indicator==0) printf(" "); else printf("A");
  printf(" ");
  out_os(MBR->Partition[i].system_indicator);
  printf(" %4i  ",MBR->Partition[i].begin_head);
  printf("%2i ",(j=MBR->Partition[i].begin_sec) & 0x3F);
  j = (j & 0xC0)<<2;
  printf("%4i ",MBR->Partition[i].begin_cyl | j);

  printf("%4i  ",MBR->Partition[i].end_head);
  printf("%2i ",(j=MBR->Partition[i].end_sec) & 0x3F);
  j = (j & 0xC0)<<2;
  printf("%4i ",MBR->Partition[i].end_cyl | j);

  printf("%10lu    ",MBR->Partition[i].preceding_sec);
  printf("%10lu\n",MBR->Partition[i].total_sec);
  }
}

int help(void)
{
printf("usage: `vmbr 0' for view of HDD 0 partition \n\
or `vmbr 1' for view of HDD 1 partition\n\n");
return 0;
}

int main (int argc, char *argv[])

{char buf [512];
int dev=0;
int hdd;

/* printf("argc=%i\n",argc); */

if (argc>1)
  switch (dev=argv[1][0])
    {
    case '0':
    case '1': dev-='0'; break;
    default:
    help();
    return 1;
    }
else
   {
   help();
   return 1;
   }

hdd=*(char far *)MK_FP(0,0x475);
printf("\nTotal HDD devices %i\n\n",hdd);
if (hdd)
  {
  if (dev<hdd)
    {
    printf("Partition table of HDD %i\n\n",dev);
    #ifdef MSDOS
    absread(0x80+dev,1,0,buf);
    #else /* ifdef Proolix */
    absread(0x80+dev,1,0,buf);
    #endif
    outMBR(buf);
    }
  }
return 0;
}
