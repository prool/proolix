#include <stdio.h>
#include <io.h>
#include <fcntl.h>

int main (int argc, char *argv[])
{int magick, h;

if (argc!=3)
{
printf("usage: bpatch filename magick\n\n\
Par example: bpatch he 0xDEAF\n");
return 1;
}

sscanf(argv[2],"%X",&magick);
printf("name=%s magick=%X",argv[1],magick);

if ((h=open(argv[1],O_WRONLY))==-1) {printf("open error\n"); return 2; }

if (write(h,&magick,2)!=2) {printf("write error\n"); return 3; }

if (close(h)==-1) {printf("close error\n"); return 4; }

return 0;
}
