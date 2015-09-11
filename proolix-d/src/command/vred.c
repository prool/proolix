#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>

void main(argc,argv)
int argc;
char *argv[];
{int h;
printf("Vred - Very Restricted EDitor\n\n");

if (argc==1)
  {
  printf("Usage: vred <filename>\n\n");
  exit(1);
  }

if ((h=open(argv[1],O_WRONLY))==-1) {perror(argv[1]); exit(2); }

printf("cat stdin to file `%s'\nCtrl-D - end of file\n\n",argv[1]);

while(1)
    {char c;
    if ((c=getchar())==4) break;
    if (write(h,&c,1)!=1) break;
    }

if (close(h)==-1) {perror(argv[1]); exit(2); }

}
