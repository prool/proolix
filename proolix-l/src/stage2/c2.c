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

void main(void)
{
char buf [BUFLEN];
int i,j;
char c,cc;
char bootsector[512];

set_color(15);

puts0("Proolix-l shell. Compiled ");
puts0(__DATE__);
puts0(" ");
puts0(__TIME__);
puts0("\r\n");

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

readboot(bootsector);

process_boot(bootsector);

out_boot(bootsector);

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
	else if (!strcmp(buf,"exit")) break;
	else if (!strcmp(buf,"quit")) break;
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
	else if (!strcmp(buf,"ls")) ls();
	else if (!strcmp(buf,"off")) off();
	else
		{
		puts0("Unknown command '");
		puts0(buf);
		puts0("'\r\n");
		}
	}

stop();
}

#include "clib.c"
