#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <io.h>
#include <dos.h>

#include <prool.h>

#define MAX_S 256

void main(argc, argv)
int argc; char *argv[];
{FILE *fp;
char s[MAX_S];
long pos, oldpos;
int i;

if (argc==1) {printf("usage: pg filename\n"); exit(0); }
#pragma warn -sus
fp=fopen(argv[1],"r");
#pragma warn +sus
L0:
pos=ftell(fp);
for(i=0;i<24;i++)
  {
  if (fgets(s,MAX_S,fp)==NULL) break;
  printf("%s",s);
  }
oldpos=pos;
pos=ftell(fp);
L1:
printf(
"\n%li: Space - next page, Minus - prev. page, 1 - Begin, ` - End, ESC - exit"
,pos);
i=getch();
putch('\r');
switch(i)
  {
  case ' ': goto L0;
  case '-': fseek(fp,oldpos,SEEK_SET); goto L0;
  case '1': fseek(fp,0,SEEK_SET); goto L0;
  case '`': fseek(fp,0,SEEK_END); goto L0;
  case ESC: break;
  default: goto L1;
  }
fclose(fp);
}
