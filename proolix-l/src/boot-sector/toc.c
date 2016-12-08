// Convertor from binary sector image 512 byte to C-source text 

#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main (int argc, char **argv)
{
unsigned short int buffer[256]; int i, fd;

if (argc!=2)
{
printf("Convertor from binary sector image 512 byte to C-source text to stdout. sizeof short int=%li bytes\r\nUsage: toc bootsector.bin\r\n", sizeof (unsigned short int));
return 1;
}

for (i=0;i<256;i++) buffer[i]=0;

fd=open(argv[1], 0);

if (fd==-1) {printf("Can't open '%s'\r\n", argv[1]); return 2;}

i=read(fd, buffer, 512);

if (i!=512) {printf("Read error '%s'\r\n", argv[1]); return 3;}

close(fd);

printf("/* Boot sector for boot from HDD. %s %s */\r\n\r\nunsigned short int boothdd [256] = {",__DATE__,__TIME__);
for (i=0;i<256;i++)
{
	printf("0x%X", buffer[i]);
	if (i<255) printf(",");
}
printf("};\r\n\r\n");
}
