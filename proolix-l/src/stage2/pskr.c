#define PROOLSKRIPT_IN_LINUX

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MAXLEN 256

#define putch(a) putchar(a)
#define puts0(STR) printf("%s",STR)
#define putdec(DIGIT) printf("%i", DIGIT)
#define puthex(DIGIT) printf("%04X", DIGIT)
#define puthex_l(DIGIT) printf("%08X", DIGIT)
#define puthex_b(DIGIT) printf("%02X", DIGIT)
#define open_ open
#define close_ close

#define O_READ 0

#include "readw.c"
#include "liblinux.c"

char *getsn(char *buf, int len)
{char *c;
fgets(buf, len, stdin);
c=strchr(buf,'\n');
if (c) *c=0;
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

int readc (int h, char *c)
{int i;
i=read(h,c,1);
if (i==1) return 0;
else return -1;
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
