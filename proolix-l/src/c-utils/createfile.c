#include "headers.h"

#define MAX_LEN_STR 100
#define FILENAME_LEN 17

void main(void)
{
unsigned short int i;
unsigned short int j;
unsigned short int equal;
unsigned char device;
unsigned char filename[FILENAME_LEN+2];
unsigned char str[MAX_LEN_STR];
int first_line;

// input filename
puts0("file ? ");
for (i=0;i<FILENAME_LEN;i++) str[i]=0;
getsn(str,MAX_LEN_STR);

for (i=0;i<FILENAME_LEN;i++) filename[i]=str[i];
filename[FILENAME_LEN]=0;

puts0("\r\n'"); puts0(filename); puts0("'\r\n");

i=open(filename,O_CREAT);
if (i==1) {puts0("open OK\r\n");}
else {puts0("open not ok\r\n"); return;}

first_line=1;

while (1)
{
// input string
puts0("data to file (] - end of text) ? ");
for (i=0;i<MAX_LEN_STR;i++) str[i]=0;
getsn(str,MAX_LEN_STR);

puts0("\r\n'"); puts0(str); puts0("'\r\n");

if (str[0]==']') break;

#if 1
if (first_line==0) writec(0,'\n');
else first_line=0;

for (i=0;i<MAX_LEN_STR;i++)
	if (str[i]) {writec(0,str[i]); putch(str[i]); }
	else break;
#endif
} // end while (1)

close(0);
}
