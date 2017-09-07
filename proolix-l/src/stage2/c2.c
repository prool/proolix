#define BUFLEN 100

#include "headers.h"

// global variables NOT USED! NOT WORK! BIKOZ .RSS SEGMENT NOT ADRESSED IN BIN FILE!

#if 0
void palette(void)
{int i;
for (i=0;i<256;i++) {if (i%16==0) puts0("\r\n");set_color(i); puts0("W");}
set_color(7); puts0("end of palette");
}
#endif

#if 0 // ЗАГЛУШКИ
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
puts0("End of code "); puthex(end_of()); puts0(" = "); putdec(end_of()); puts0("\r\n");
}

void main(void)
{
char buf [BUFLEN];
int i,j;
char c,cc;
char bootsector[512];
unsigned short int drive, reg_bx, reg_cx, reg_dx;
unsigned short int cyl, sectors, heads, total_sec;

//putch('/');
putch_color('@', 4);

version();

time();

print_boot();

// test disk drives

for (i=0;i<4;i++)
	{ // i = 0, 1, 2, 3
	if (i==2) drive=0x80; else if (i==3) drive=0x81; else drive=i;
	// drive = 0, 1, 0x80, 0x81
	reg_bx=GetDriveParam_bx(drive);
	//puts0("*");
	if (reg_bx==0xFFFF) {
		gCyl[i]=0;
		gSec[i]=0;
		gHeads[i]=0;
		gTotal[i]=0;
		//puts0("z");
		continue;}

	reg_cx=GetDriveParam_cx(drive);
	reg_dx=GetDriveParam_dx(drive);


	cyl = (((reg_cx & 0xFF00U)>>8)&0xFFU) | ((reg_cx & 0xC0U)<<2);

	sectors = reg_cx &0x3FU;

	heads = ((reg_dx & 0xFF00U) >> 8)&0xFFU;
	heads++;

	total_sec=(sectors*heads*(cyl+1));

	gCyl[i]=cyl;
	gSec[i]=sectors;
	gHeads[i]=heads;
	gTotal[i]=total_sec;
	}

puts0("Cyl Sec Heads Total\r\n");
for (i=0;i<4;i++)
{
putdec(gCyl[i]);
puts0(" ");
putdec(gSec[i]);
puts0(" ");
putdec(gHeads[i]);
puts0(" ");
putdec(gTotal[i]);
puts0("\r\n");
}

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
	else if (!strcmp(buf,"cold")) cold();
	else if (!strcmp(buf,"hdd0")) hdd0();
	else if (!strcmp(buf,"hdd1")) hdd1();
	else if (!strcmp(buf,"fdd"))  fdd();
	else if (!strcmp(buf,"test")) test();
	else if (!strcmp(buf,"ascii")) ascii();
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
	else if (!strcmp(buf,"format")) format();
	else if (!strcmp(buf,"super")) view_superblock();
	else if (!strcmp(buf,"ls")) ls();
	else if (!strcmp(buf,"create")) create_file3();
	else if (!strcmp(buf,"tofile")) tofile();
	else if (!strcmp(buf,"tofile2")) tofile2();
	else if (!strcmp(buf,"rm")) remove_file();
	else if (!strcmp(buf,"cat")) cat();
	else if (!strcmp(buf,"dd")) dd();
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
