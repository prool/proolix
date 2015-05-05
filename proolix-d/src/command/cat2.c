/* Программа для отладки вызовов open и read */
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

void main(int argc, char *argv[])
{int h1, h2, n;
char c;

if (argc<2) {puts("Use cat2 <file1> [<file2>]");exit(1);}

puts(argv[1]);
puts(argv[2]);

if ((h1=open(argv[1],O_RDONLY))==-1)
  {
  printf("Can't open %s\n",argv[1]);
  return;
  }

if ((h2=open(argv[2],O_RDONLY))==-1)
  {
  printf("Can't open %s\n",argv[2]);
  }

while(1)
{
while(1)
  {
  n=(int)read(h1,&c,1);
  if (n!=1) {puts("Can't read 1");exit(2);}
  else putch(c);
  if(c=='\n') break;
  }
if(getch()==3)exit(0);
while(1)
  {
  n=(int)read(h2,&c,1);
  if (n!=1) puts("Can't read 2");
  else putch(c);
  if(c=='\n') break;
  }
if(getch()==3)exit(0);
}
}
