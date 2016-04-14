#define BUFLEN 100

#include "headers.h"

// global variables NOT USED! NOT WORK! BIKOZ .RSS SEGMENT NOT ADRESSED IN BIN FILE!

void testchar(char *c)
{
*c='c';
}

void palette(void)
{int i;
for (i=0;i<256;i++) {if (i%16==0) puts0("\r\n");set_color(i); puts0("W");}
set_color(7); puts0("end of palette");
}

#if 1 // ЗАГЛУШКИ
void ls(void)
{
puts0("no ls!\r\n");
}
int open (char *filename, int flag)
{
return -1;
}
int close(int descriptor)
{
return 0;
}
int readw(int fd, char *buf, int count)
{
return 0;
}
#endif

void main(void)
{
char buf [BUFLEN];
int i,j;
char c,cc;
char bootsector[512];

current_drive=0xFF;

set_color(15);

puts0("\r\nProolix-l shell. Compiled ");
puts0(__DATE__);
puts0(" ");
puts0(__TIME__);
puts0("\r\n");

puts0("AX = ");
puthex(boot_drive);
puts0("\r\n");

puts0("? - for help\r\n");

#if 0 // test of putdec()
puts0("digit 0. putdec: "); putdec(0); puts0("\r\n");
puts0("digit 1. putdec: "); putdec(1); puts0("\r\n");
puts0("digit 50. putdec: "); putdec(50); puts0("\r\n");
puts0("digit 55. putdec: "); putdec(55); puts0("\r\n");
puts0("digit 100. putdec: "); putdec(100); puts0("\r\n");
puts0("digit 101. putdec: "); putdec(101); puts0("\r\n");
puts0("digit -1. putdec: "); putdec(-1); puts0("\r\n");
#endif

set_color(6);

puts0("End of code "); puthex(end_of()); puts0("\r\n");

set_color(7);

#if 0
readboot(bootsector);
process_boot(bootsector);
out_boot(bootsector);
mount_disk(0);
#endif

if (boot_drive==0xDDDDU)
	{
	puts0("Boot from HDD0\r\n");
	mount_disk(0x80U);
	}
else if (boot_drive==0xAAAAU)
	{
	puts0("Boot from FDD0\r\n");
	mount_disk(0);
	}
else puts0("Boot from unknown device\r\n");

//puts0("cursor coord "); puthex(get_row()); puts0(" "); puthex(get_col()); puts0("\r\n"); 

#if 0
puts0("get color "); puthex(get_color()); puts0("\r\n");
set_color(1);
puts0("get color "); puthex(get_color()); puts0("\r\n");
set_color(2);
puts0("get color "); puthex(get_color()); puts0("\r\n");
#endif

while (1)
	{
	puts0("\r\nEnter command -> ");
	getsn(buf,BUFLEN);
	puts0("\r\n");

	if (buf[0]==0) puts0("");
	else if (!strcmp(buf,"help")) help();
	else if (!strcmp(buf,"?")) help();
	else if (!strcmp(buf,"test")) test();
	else if (!strcmp(buf,"ascii")) ascii();
	else if (!strcmp(buf,"cls")) cls();
	else if (!strcmp(buf,"scroll")) scroll();
	else if (!strcmp(buf,"palette")) palette();
	else if (!strcmp(buf,"system")) system();
	else if (!strcmp(buf,"memd0")) memd0();
	else if (!strcmp(buf,"memd")) memd();
	else if (!strcmp(buf,"memmap")) memmap();
	else if (!strcmp(buf,"basic")) basic();
	else if (!strcmp(buf,"diskd0")) diskd0();
	else if (!strcmp(buf,"diskd")) diskd();
	else if (!strcmp(buf,"testdisk")) testdisk();
	else if (!strcmp(buf,"mount")) mount();
	else if (!strcmp(buf,"off")) off();
	else if (!strcmp(buf,"vec")) vectors();
	else if (!strcmp(buf,"skript")) skript();
	else
		{
		puts0("Unknown command '");
		puts0(buf);
		puts0("'\r\n");
		}
	}

puts0("QUIT\r\n");
stop();
}

#include "clib.c"
