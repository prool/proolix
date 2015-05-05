#define IDENT "Proolix Kernel's internal shell\n\
Type `help' for help\nType `s cmd' for start multitask\n\
Type `q' for forced boot from 1st HDD\n"

/****************************************************************************/
#define ALLOW_TRACE 0 /* 0 1 */
/* #define DEBUG */
/****************************************************************************/
#include <prool.h>

#include <conf.c>
#include "kernel.h"

#include "signal.h"

/****************************************************************************/
void null_command(void);
void run_boot(int dev,int head,int sec,int trk);
void start(char *cmd);
void stimer(int argc, char far *argv[]);
/****************************************************************************/
struct internal_commands
  {
  char far *command_name;
  void (*command_entry) (void);
  }
InternalCommand [] = {
{"cls", clrscr},
{"clear", clrscr},
{"dev", out_devices},
/* {"exitw", exitw}, */
{"", null_command}
};
/****************************************************************************/
void null_command (void)
{
/* null action */
return;
}
/****************************************************************************/
int print_exec_message(int e)
{
switch(e)
  {
  case 1 : printf("%Fs: not found\n",_argv[0]);       break;
  case 2 : printf("%Fs: not executable\n",_argv[0]);  break;
  case 3 : printf("%Fs: read error\n",_argv[0]);      break;
  case 4 : printf("%Fs: memory fault\n",_argv[0]);    break;
  case 5 : printf("%Fs: no mem\n",_argv[0]);
  default: ;
  case 0 : ;
  }
return e;
}
/****************************************************************************/
void garbage_collection(void)
{
if (!garbage()) puts("\nGarbage collected");
else puts("\nNo fragmentation");
}
/****************************************************************************/
static void fcbs(void)
{int i, j; char c;

printf(
"No ---name---- flag T    Beg Size   D ino Seek ACl RCl ASc RSc ofset\n");
for (i=0;i<OPEN_MAX;i++)
  {
  printf("%2i ",i);
  for (j=0;j<11;j++) if ((c=FCBS[i].Name[j])==0) putch ('·'); else putch(c);
  printf(" %4X %1X %5i %6li %1X %3i %4lX %6i %6i %5li\n",
  FCBS[i].flag, FCBS[i].File, FCBS[i].BegClu, FCBS[i].Size, FCBS[i].Drive,
  FCBS[i].DirInode, FCBS[i].DirSeekPos, FCBS[i].AbsClu,
  FCBS[i].AbsSec, FCBS[i].AbsOffset);
  }
}
/****************************************************************************/
static void md (int _argc, char far *_argv[])
{
if (_argc==2) if (mkdir(_argv[1],0)) printf("Can't make %Fs",_argv[1]);
}
/****************************************************************************/
static void cd (int _argc, char far *_argv[])
{char far *dir;
if (_argc==1)
  { /* cd homedir */
  dir=getenv("HOME");
  if (dir==NULL) return ;
  }
else
  dir=_argv[1];
if (chdir(dir)) printf("Can't chdir %Fs",dir);
}
/****************************************************************************/
static void pwd(void)
{
char s[PATH_MAX+1];
if (getcwd(s,PATH_MAX+1)==NULL)
  puts("Error");
else
  puts(s);
}
/****************************************************************************/
static void buf(void)
{int i;

for (i=0;i<BUFFERS;i++)
  {int j;
  long j1;
  if((j=(*(Cache+i)).Status)==0) continue;
  printf("%3i %3li",i,j1=(*(Cache+i)).Sec);
  if (j1==0) printf(" BOT ");
  else if (j1<RootBeg) printf(" FAT ");
  else if (j1<DataStart) printf(" ROT ");
  else printf(" DAT ");
  switch(j)
    {
    case 1: printf("   "); break;
    case 2:
    default:printf(" * ");
    }
  printf("%6i\n",(*(Cache+i)).Counter,6,0);
  }
}
/****************************************************************************/
static void call_mount(int argc, char far *argv[])
{
if (argc==1)
  {
  mount(-1);
  printf("\nUse: `mount digit' or `mount dos_letter'for new device mount\n\n\
  digit:\n\
  FDD:\n\
  0 - A:\n\
  1 - B:\n\
  HDD:\n\
  2 - C: ...\n\
  4 - D:\n\
  6 - E:\n\
  ...\n");
  }
else
  {
  ReadOnly=0;
  if (argv[1][0]<'9') mount(argv[1][0]-'0');
  else mount((toupper(argv[1][0])-'C')*2+2);
  }
}
/****************************************************************************/
static void SetMore(int argc, char far *argv[])
{
int i;

for(i=1;i<=argc;i++)
  {
  if (argv[i][0]=='-')
    switch (argv[i][1])
      {
      case 'y': More=1; return;
      case 'n': More=0; return;
      }
  }
printf("More=%i\nUse more [-y]|[-n] for set more on|off\n",More);
}
/*****************************************************************************/
#if ALLOW_TRACE
static void trace(int argc, char far *argv[])
{
int i;

for(i=1;i<=argc;i++)
  {
  if (argv[i][0]=='-')
    switch (argv[i][1])
      {
      case 'y': Trace=1; return;
      case 'n': Trace=0; return;
      }
  }
printf("Trace=%i\nUse trace [-y]|[-n] for set trace on|off\n",Trace);
}
#endif
/*****************************************************************************/
static void xonix(void)
{
#define MaxX 79
#define MaxY 24
#define MinX 1
#define MinY 1
#define Slowly 90000L
/* #define Trace */
int x,y;
int dx,dy;
#ifndef Trace
int oldx,oldy;
#endif
#ifdef Slowly
long ii;
#endif

clrscr();

x=2;
y=2;
dx=1;
dy=1;
gotoxy(x,y);
putch('*');

while(1)
{
if (kbhit())break;
if (x==MaxX) dx=-dx; else if (x==MinX) dx=-dx;
if (y==MaxY) dy=-dy; else if (y==MinY) dy=-dy;
#ifndef Trace
oldx=x;oldy=y;
#endif
x+=dx; y+=dy;
#ifndef Trace
gotoxy(oldx,oldy);putch(' ');
#endif
gotoxy(x,y);putch('*');
#ifdef Slowly
for (ii=1;ii<Slowly;ii++);
#endif
}
getch();

}
/****************************************************************************/
static void date (void)
{
struct date d;
struct time t;

getdate(&d);
gettime(&t);

printf("Date is %2i-%02i-%02i %2i:%02i:%02i\n\n",
d.da_day,d.da_mon,d.da_year+1980,
t.ti_hour,t.ti_min,t.ti_sec);
}
/****************************************************************************/
static void echo(void)
{int i=1;
while (_argv[i]) printf("%s ",_argv[i++]);
}
/****************************************************************************/
static void put_env(void)
{
int i;
for (i=0; pEnv[i] != NULL; i++)
 {
  puts (pEnv[i]);
 }
}
/****************************************************************************/
char Clipboard [PATH_MAX+1];
/****************************************************************************/
void external_command(void)
{int i=0;
char far *pathl, far *cc;

/* Временно в качестве заплаты (пока переделывается поддержка среды)
подставляем фиксированное значение PATH */
#if 0
if ( (pathl=getenv("PATH"))==NULL)
  {
  print_exec_message(execve(_argv[0],_argv,pEnv));
  return ;
  }
#else
pathl=":/bin";
#endif

for(cc=pathl;;pathl++)
  {
  if ((*pathl==':')||(*pathl==0))
    {
    if (cc==pathl)
      {/* ищем в текущем каталоге */
      i=execve(_argv[0],_argv,pEnv);
      if (!i) return;
      }
    else
      {/* ищем в указанном каталоге */
      memcpy(Clipboard,cc,i=(int)(pathl-cc));
      Clipboard[i]=0;
      strcat(Clipboard,"/");
      strcat(Clipboard,_argv[0]);
      i=execve(Clipboard,_argv,pEnv);
      if (!i) return;
      }
    if (*pathl==0) break;
    cc=pathl+1;
    }
  }
if (i>1) print_exec_message(i);
else printf("%s: not found or not executable\n",_argv[0]);
return ;
}

void boot(char far *argv[])
{int d;
if (shutdown())
  {
  printf("Shutdown error!\n");
  return;
  }
d=toupper(argv[1][0])-'A';
printf("boot from %c:\n",dos_letter(d));
switch(d)
  {
  case  0:
  case  1: run_boot(d,0,1,0);
  default: if (Devices[d].sec==0)
             {printf("Device %c: not present\n",dos_letter(d));return;}
           run_boot(0x80,Devices[d].head,Devices[d].sec,Devices[d].trk);
  }
}
/*****************************************************************************
                                    sh()
*****************************************************************************/
char sh_str_orig [MAX_LEN_STR];
char sh_str [MAX_LEN_STR];

void interrupt far sh (void)
{
int i;
puts(IDENT);

while(1)
{
/* Вывод символа-приглашения shell'а и ввод командной строки */
#ifndef DOSKEY
for (i=0;i<MAX_LEN_STR;i++) {sh_str_orig[i]=0; sh_str[i]=0; }
printf("-> ");
gets(sh_str_orig);

  #ifdef DEBUG
  printf("sh:");
  printf(sh_str_orig);
  #endif

#else
printf("% ");
doskey(sh_str_orig);
#endif

#if 0
if (!str[0])
goto shut;
#endif

strncpy(sh_str,sh_str_orig,MAX_LEN_STR);

/* Разбор командной строки */
i=0; while (sh_str[i]) if (sh_str[i]=='\r') {sh_str[i]=0; break;} else i++;

_argc=0;
i=0;

/* Пропуск пробелов в начале командной строки */
for(;(i<PassLen-1)&&(sh_str[i]);i++)
  if((sh_str[i]!=' ')&&(sh_str[i]!='\t'))break;

/* Разбор командной строки на параметры */
for(;(i<PassLen-1)&&(sh_str[i]);)
  {
  _argv[_argc++]=sh_str+i;
  if (_argc>MAX_ARG) {printf("Too many arguments"); goto l2;}
  for(;(i<PassLen-1)&&(sh_str[i]);i++)
    if((sh_str[i]==' ')||(sh_str[i]=='\t')) break;
  if (!sh_str[i])break;
  sh_str[i++]=0;
  for(;(i<PassLen-1)&&(sh_str[i]);i++)
    if((sh_str[i]!=' ')&&(sh_str[i]!='\t')) break;
  }
if (_argc)_argv[_argc]=NULL; else goto l2;

/* Поиск по списку внутренних комманд.
   Данный поиск реализован в виде вложенных операторов if и этот if уже дошел
   почти до предела возможностей компилятора Turbo C 2.0. (Попробуйте
   добавить еще команд и посмотрите, что получится */

#ifdef DEBUG
printf(" sh: _argv[0]=");
printf(_argv[0]);
#endif

if(!strcmp(_argv[0],"exit")) goto shut;
else if(!strcmp(_argv[0],"shutdown"))
  {
  shut:
  #if 0
  puts("Press any key for exit or ESC for return to Proolix");
  if (getch()!=ESC)
  #endif
    {int i=0, j;
    while ((j=shutdown())!=0)
      {
      puts("Shutdown error\n\nPress ESC for return to Proolix or \
      \n'B' for exit without shutdown or other key for retry");
      i=getch();
      if (i=='B') {j=0; break;}
      if (i==ESC) break;
      }
    if (j==0) {if (emulation_mode==0) reboot(); else exitw(); }
    }
  }
/* Поиск по списку внутренних комманд (продолжение) */
else if (!strcmp(_argv[0],"q")) goto l_msdos;
else if (!strcmp(_argv[0],"msdos"))
  {l_msdos:
  if (emulation_mode!=0) exitw();
  else {_argv[1][0]='c'; boot(_argv); } }
else if (!strcmp(_argv[0],"boot")) {if (_argc==1) goto shut; boot(_argv); }
/* else if(!strcmp(_argv[0],"ver")) ident(); */
/* else if(!strcmp(_argv[0],"#")) ; */
else if(!strcmp(_argv[0],"sync")) {if(FlushAll()) puts("flush error");}
else if(!strcmp(_argv[0],"l")) l(_argc,_argv);
else if(!strcmp(_argv[0],"rm")) rm(_argc,_argv);
/* else if(!strcmp(_argv[0],"Rm")) rm(_argc,_argv); */
else if(!strcmp(_argv[0],"mv")) mv(_argc,_argv);
else if(!strcmp(_argv[0],"Cp")) cp(_argc,_argv);
else if(!strcmp(_argv[0],"E")) e (_argc,_argv);
else if(!strcmp(_argv[0],"mem")) mem();
else if(!strcmp(_argv[0],"memd")) memd();
/* else if(!strcmp(_argv[0],"alloc")) alloc(sh_str+6); */
/* else if(!strcmp(_argv[0],"free")) kill_zombie(sh_str+5); */
else if(!strcmp(_argv[0],"garbage")) garbage_collection();
/* else if(!strcmp(_argv[0],"open")) open_(sh_str+5); */
else if(!strcmp(_argv[0],"fcb")) fcbs();
else if(!strcmp(_argv[0],"Cat")) cat(_argc,_argv);
else if(!strcmp(_argv[0],"Vec")) vec(_argc,_argv);
else if(!strcmp(_argv[0],"int0")) geninterrupt(0);
else if(!strcmp(_argv[0],"echo")) echo();
else if(!strcmp(_argv[0],"de")) de();
else if(!strcmp(_argv[0],"env")) put_env();
/* else if(!strcmp(_argv[0],"timer")) stimer(_argc,_argv); */
else if(!strcmp(_argv[0],"date")) date();
else if(!strcmp(_argv[0],"Xonix")) xonix();
else if(!strcmp(_argv[0],"buf")) buf();
else if(!strcmp(_argv[0],"mount")) call_mount(_argc,_argv);
else if(!strcmp(_argv[0],"umount")) umount();
else if(!strcmp(_argv[0],"setmore")) SetMore(_argc,_argv);
#if ALLOW_TRACE
else if(!strcmp(_argv[0],"trace")) trace(_argc,_argv);
#endif
else if(!strcmp(_argv[0],"pwd")) pwd();
else if(!strcmp(_argv[0],"cd")) cd(_argc,_argv);
/* else if(!strcmp(_argv[0],"chdir")) cd(_argc,_argv); */
else if(!strcmp(_argv[0],"md")) md(_argc,_argv);
else if(!strcmp(_argv[0],"mkdir")) md(_argc,_argv);
else if(!strcmp(_argv[0],"start")) start(sh_str+6);
else
  { /* Поиск в таблице внутренних команд */
  #ifdef DEBUG
  printf("internal commands\n");
  #endif
  for (i=0;;i++)
    {
    #ifdef DEBUG
    printf("TRACE SH\n");
    #endif
    if (*(InternalCommand[i].command_name)==0) break;
    #ifdef DEBUG
    printf("command=%s\n",InternalCommand[i].command_name);
    #endif
    if (!strcmp(_argv[0],InternalCommand[i].command_name))
      {
      InternalCommand[i].command_entry();
      goto l2;
      }
    }
  goto l3;
  }

continue;

l3:
/* Hайденная команда, скорее всего, внешняя */
if (strchr(_argv[0],'/')==NULL)
  {
  #if 1 /* поиск по пути PATH */
  external_command();
  #else
  print_exec_message(execve(_argv[0],_argv,pEnv));
  #endif
  }
else
  /* В имени команды есть символ[ы] '/', значит, это имя команды с
      явно указанным путем. Передаем это имя на выполнение без поиска по
      путям PATH */
  print_exec_message(execve(_argv[0],_argv,pEnv));

l2: ;
} /* while (1) */
} /* sh() */