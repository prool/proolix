#define BUFLEN 100

#include "headers.h"

#include "clib.c"

// global variables NOT USED! NOT WORK! BIKOZ .RSS SEGMENT NOT ADRESSED IN BIN FILE!

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

set_color(15);

puts0("Proolix-l shell. Compile ");
puts0(__DATE__);
puts0(" ");
puts0(__TIME__);
puts0("\r\n");

set_color(7);

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
	else if (!strcmp(buf,"exit")) break;
	else if (!strcmp(buf,"quit")) break;
	else if (!strcmp(buf,"test")) test();
	else if (!strcmp(buf,"ascii")) ascii();
	else if (!strcmp(buf,"cls")) cls();
	else if (!strcmp(buf,"scroll")) scroll();
	else if (!strcmp(buf,"palette")) palette();
	else
		{
		puts0("Unknown command '");
		puts0(buf);
		puts0("'\r\n");
		}
	}

stop();
}

void help(void)
{
puts0("Proolix-l shell command\r\n\r\n\
test - test\r\n\
help - help\r\n\
ascii - write ascii table\r\n\
cls - clearscreen\r\n\
palette - print color palette\r\n\
exit, quit - exit\r\n\
");
}
