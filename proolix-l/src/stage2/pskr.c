#include <stdio.h>
#include <string.h>

#define MAXLEN 256
#define puts0(STR) printf("%s",STR)
#define putdec(DIGIT) printf("%i", DIGIT)
#define puthex(DIGIT) printf("%04X", DIGIT)
#define puthex_l(DIGIT) printf("%08X", DIGIT)
#define puthex_b(DIGIT) printf("%02X", DIGIT)
#define peek(ADR) 0
#define peek2(SEG,OFFSET) 0

#include "readw.c"

char *getsn(char *buf, int len)
{char *c;
fgets(buf, len, stdin);
c=strchr(buf,'\n');
if (c) *c=0;
}

int reads(int fd, char *buf, int count) // read string from file
{
int i, j; char c;

for (i=0;i<(count-1);i++)
	{
	j=read(fd,&c,1);
	if ((j==0)||(c=='\r')||(c=='\n')) {*buf=0; return 1;}
	*buf++=c;
	}
*buf=0;
return 0;
}

int htoi(const char  *s)
{
int i; char c;
i=0;
while (*s)
  {
  i<<=4;
  c=toupper(*s++);
  if ((c>='0')&&(c<='9')) i+=c-'0';
  else if ((c>='A')&&(c<='F')) i+=c+10-'A';
  else return 0;
  }
return i;
}

void ls(void)
{
printf("ls not implemented yet\n");
}

#include "proolskript.c"

void main(void)
{
printf("ProolSkript Interpreter\n");

skript();

puts("");
}
