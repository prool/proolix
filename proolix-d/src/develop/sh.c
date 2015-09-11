/* #define DEBUG */

#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <string.h>
#include <process.h>
#include <proto.h>
#include <dirent.h>

#include "prool.h"

#include "conf.c"
#include "limits.h"
#include "unistd.h"
#include "struct.h"

#include "zadroka.h"

int p;
int _argc;
char far*_argv [MAX_ARG];
char str[MAX_LEN_STR];
char far * far * _env;
char far* __env [MAX_ENV];
/*****************************************************************************/
void date (void)
{
struct date d;
struct time t;

getdate(&d);
gettime(&t);

printf("Date is %2i-%02i-%02i %2i:%02i:%02i\n\n",
d.da_day,d.da_mon,d.da_year+1980,
t.ti_hour,t.ti_min,t.ti_sec);
}
/*****************************************************************************/
void echo(void)
{int i=1;
while (_argv[i]) printf("%s ",_argv[i++]);
}
/*****************************************************************************/
void put_env(char far*far*_env)
{
int i;
for (i=0; _env[i] != NULL; i++)
 {
  puts (_env[i]);
 }
}
/*****************************************************************************/
void ladder (void)
{int i,j;
for (;;)
  {
  for (i=0;i<80;i++)
    {
    for (j=0;j<i;j++) putch(' ');
    puts("-");
    for (j=0; j<32000; j++);
    if (kbhit()) {getch(); return;}
    }
  }
}
/****************************************************************************/
int main (void /*int argc, char argv[], char **env*/)
{
int i;

puts("Proolix external shell\n");

for (i=0;i<MAX_ENV;i++)
  __env[i]=NULL;

_env=__env;
*(_env+ 0)="HOME=/";
*(_env+ 1)=NULL;

/*_str=(char far *)str;*/

while(1)
  {
  printf("$ ");
  gets(str);

  puts("");

  if (str[0]==0) continue;
  if (str[0]=='\r') continue;

  i=0; while (str[i]) if (str[i]=='\r') {str[i]=0; break;} else i++;

  _argc=0;
  i=0;

  /* пропуск пробелов в начале командной строки */
  for(;(i<PassLen-1)&&(str[i]);i++)
    if((str[i]!=' ')&&(str[i]!='\t'))break;

  /* разбор командной строки */
  for(;(i<PassLen-1)&&(str[i]);)
    {
    _argv[_argc++]=str+i;
    if (_argc>MAX_ARG) {printf("Too many arguments"); goto l2;}
    for(;(i<PassLen-1)&&(str[i]);i++)
      if((str[i]==' ')||(str[i]=='\t')) break;
    if (!str[i])break;
    str[i++]=0;
    for(;(i<PassLen-1)&&(str[i]);i++)
      if((str[i]!=' ')&&(str[i]!='\t')) break;
    }
  if(_argc)_argv[_argc]=NULL;else goto l2;

  i=0;
  while (*Action[i][0])
  {
  if (!strcmp(_argv[0],Action[i][0])) {printf(Action[i][1]); goto l2;}
  i++;
  }

  #ifdef DEBUG
  printf("Trace1 ");
  #endif

  if (!_argv[0][0]) printf("Null command\n");

  else if (!strcmp(_argv[0],"cls")) goto cls;
  else if (!strcmp(_argv[0],"clear")) cls: clrscr();
  else if (!strcmp(_argv[0],"echo")) echo();
  else if (!strcmp(_argv[0],"set")) put_env(_env);
  else if (!strcmp(_argv[0],"date")) date();
  else if (!strcmp(_argv[0],"ps")) service(1);
  else
    {
    p=fork();
    if (p==-1) puts("sh: sorry, can't fork");
    else if (p)
      { /* parent */
      /* printf("wait on\n"); */
      wait(p);
      putch('W');
      /* printf("wait off\n"); */
      /* l0: goto l0; */
      }
    else
      { /* child */
      /* printf("exec on\n"); */
      exec(_argv[0]);
      printf("sh: exec error\n");
      exit(1);
      }
    putch('<');
    }
  l2:;
  }
ret:
return 0;
}
