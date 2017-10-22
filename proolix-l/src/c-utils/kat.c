// kat

// katenate file to stdout (similar to Linux' cat)

#include "headers.h"

#define MAX_LEN_STR 100
#define FILENAME_LEN 17

void main(void)
{
unsigned short int i;
unsigned short int j;
unsigned short int equal;
unsigned char filename[FILENAME_LEN+2];
unsigned char str[MAX_LEN_STR];
char c;
int rc;
int line=0;

// ввод имени файла
puts0("file ? ");
for (i=0;i<FILENAME_LEN;i++) str[i]=0;
getsn(str,MAX_LEN_STR);

for (i=0;i<FILENAME_LEN;i++) filename[i]=str[i];
filename[FILENAME_LEN]=0;

//puts0("\r\n'"); puts0(filename); puts0("'\r\n");

rc=open(filename,O_READ);
//puthex(rc);
if (rc!=-1) {/*puts0("open OK\r\n");*/}
else {puts0("can't open file\r\n"); return;}

while(readc(0,&c)==0)
	if (c=='\n')
		{
		puts0("\r\n");
		if (++line>23)
			{puts0(" MORE ");puts0(filename);
			c=getchar();
			if (c=='q') {close(rc); return;}
			line=0;putch('\r');for (i=0;i<70;i++)putch(' ');putch('\r');}
		}
	else putch(c);

close(0);
}
