#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include "viewexe.c"

#define BUFLEN 512

void ident (void)
{
printf("View EXE ver. 0.0.0.1  9-Mar-96 (c) by Prool\n\n");
}

void main (int argc, char *argv[])

{char buf [BUFLEN]; int h, i;

if (argc!=2)
  {
  ident();
  printf("usage: vexe filename");
  }
else
  {
  if ((h=open(argv[1],O_RDONLY | O_BINARY))==-1)
    printf("Can't open %s\n",argv[1]);
  if ((i=read(h,buf,BUFLEN))!=BUFLEN)
    printf("Can't read %s. read()=%i\n",argv[1],i);
  close(h);
  printf("File %s\n\n",argv[1]);
  ViewEXE((void *)buf);
  printf("\n\n");
  ident();
  }
}
