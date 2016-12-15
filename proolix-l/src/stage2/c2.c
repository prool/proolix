#define BUFLEN 100

#include "headers.h"

extern unsigned short int FCB;

// global variables NOT USED! NOT WORK! BIKOZ .RSS SEGMENT NOT ADRESSED IN BIN FILE!

void testchar(char *c)
{
*c='c';
}

#if 0
void palette(void)
{int i;
for (i=0;i<256;i++) {if (i%16==0) puts0("\r\n");set_color(i); puts0("W");}
set_color(7); puts0("end of palette");
}
#endif

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

void time(void)
{
	short int ii=get_rtc();
	puts0("time ");
	puthex(ii);

	ii=get_day();
	puts0(" day ");
	puthex(ii);

	ii=get_year();
	puts0(" year ");
	puthex(ii);
	puts0("\r\n");

}

void version (void)
{
puts0("\r\nProolix-l shell. Compiled ");
puts0(__DATE__);
puts0(" ");
puts0(__TIME__);
puts0("\r\n");
//pause();
puts0("? - for help\r\n");
puts0("End of code "); puthex(end_of()); puts0("\r\n");
}

void main(void)
{
char buf [BUFLEN];
int i,j;
char c,cc;
char bootsector[512];

#if 0
puts0("FCB=");
puthex(FCB);
#endif

//putch('/');
putch_color('@', 4);

version();

time();

//puts0("cursor coord "); puthex(get_row()); puts0(" "); puthex(get_col()); puts0("\r\n"); 

while (1)
	{
	puts0("\r\nEnter command -> ");
	getsn(buf,BUFLEN);
	puts0("\r\n");

	if (buf[0]==0) puts0("");
	else if (!strcmp(buf,"ver")) version();
	else if (!strcmp(buf,"help")) help();
	else if (!strcmp(buf,"?")) help();
	else if (!strcmp(buf,"reboot")) reboot();
	else if (!strcmp(buf,"test")) test();
	else if (!strcmp(buf,"ascii")) ascii();
	else if (!strcmp(buf,"cls")) cls();
	else if (!strcmp(buf,"scroll")) scroll();
	else if (!strcmp(buf,"system")) system();
	else if (!strcmp(buf,"memd0")) memd0();
	else if (!strcmp(buf,"memd")) memd();
	else if (!strcmp(buf,"memmap")) memmap();
	else if (!strcmp(buf,"basic")) basic();
	else if (!strcmp(buf,"diskd0")) diskd0();
	else if (!strcmp(buf,"diskd")) diskd();
	else if (!strcmp(buf,"off")) off();
	else if (!strcmp(buf,"vec")) vectors();
	else if (!strcmp(buf,"skript")) skript();
	else if (!strcmp(buf,"scr")) screensaver();
	else if (!strcmp(buf,"time")) time();
	else if (!strcmp(buf,"install")) install();
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
