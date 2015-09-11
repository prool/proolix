/*************************BEGIN**********************************************/
/* #define DEBUG */

#include <prool.h>
#include <conf.c>
#include "kernel.h"

void start(char *cmd);

unsigned char c=0;
unsigned int unique_pid=1;
jmp_buf int8_point;
int current_pid=0;
int sema_ps=0;
/****************************************************************************/
int sema_lock (int far *sema)
{
__cli__();
if (*sema==0)
  {
  *sema=1;
  __sti__();
  return 0; /* locked OK */
  }
__sti__();
return 1; /* no locked */
}
/****************************************************************************/
void sema_unlock (int far *sema)
{
__cli__();
if (*sema==1) *sema=0;
__sti__();
}
/****************************************************************************/
int generate_pid (void)
{
return unique_pid++;
}
/****************************************************************************/
int load_file(char far *name, char huge *adr)
{int h, i;

#ifdef DEBUG
printf("load_file: `%Fs'\n",name);
#endif

if ((h=open(name,O_RDONLY))==-1)
  {
  printf("load_file: Can't open `%Fs'\n",name);
  return 1;
  }

while (1)
  {
  i=(int)read(h, (void far *)adr, SECTOR_SIZE);
  if (i!=SECTOR_SIZE) break;
  adr+=SECTOR_SIZE;
  }

close(h);
return 0;
}
/****************************************************************************/
void ps_ (void)
{int i;
printf("current_pid=%i\n",current_pid);
printf(
"# PID status mem0     cs   ip   ds   es   ax   ss   sp   flag cmd stacktop\n"
);
for (i=0;i<PROC_MAX;i++)
  {int PID, j;
  PID=process_table[i].pid;
  if (PID)
    {
    printf(
    "%1i %3i %1i      %08lX %04X %04X %04X %04X %04X %04X %04X %04X",
    i,PID,process_table[i].status,
    process_table[i].mem0,
    process_table[i].control_point[0].j_cs,
    process_table[i].control_point[0].j_ip,
    process_table[i].control_point[0].j_ds,
    process_table[i].control_point[0].j_es,
    process_table[i].control_point[0].j_ax,
    process_table[i].control_point[0].j_ss,
    process_table[i].control_point[0].j_sp,
    process_table[i].control_point[0].j_flag);
    j=*(char far *)MK_FP /* cmd */
      (process_table[i].control_point[0].j_cs,
      process_table[i].control_point[0].j_ip);
    printf(" %02X",j);
    j=*(int far *)MK_FP /* stacktop */
      (process_table[i].control_point[0].j_ss,
      process_table[i].control_point[0].j_sp);
    printf(" %04X",j);
    printf("\n");
    }
  }
}
/****************************************************************************/
int process_position (int p)
{int i;
for (i=0;i<PROC_MAX;i++)
  {
  if (process_table[i].pid==p)
    {
    return i;
    }
  }
return -1;
}
/****************************************************************************/
int new_position(void)
{int i;
while (sema_lock(&sema_ps)==1);
for (i=0;i<PROC_MAX;i++)
  {
  if (process_table[i].pid==0)
    {
    process_table[i].status=0;
    process_table[i].pid=generate_pid();
    sema_unlock(&sema_ps);
    return i;
    }
  }
printf("process table overflow\n");
sema_unlock(&sema_ps);
return -1;
}
/****************************************************************************/
int fork (void)
/* fork returned 0 to child process;
                 childPID to parent process;
                 -1 if error                                                */
{int i, parent_pos, child_pos;
char huge *mem0;
long len;

/*      Stack:

        BP      + 0             SP = BP         user_ss:user_sp
        DI      + 2
        SI      + 4
        DS      + 6
        ES      + 8
        DX      +10
        CX      +12
        BX      +14
        AX      +16
        IP      +18     ;
        CS      +20     ; int 84 (int INT_NO)
        FLAGS   +22     ;
        RET ADR +24     call fork()
                                                                            */
while (sema_lock(&sema_ps)==1);
parent_pos=process_position(current_pid);
process_table[parent_pos].status=0;
sema_unlock(&sema_ps);

/* создаем новый пустой процесс */
if ((child_pos=new_position())==-1) return -1;

len=((long)par_size_of_block((void far *)process_table[parent_pos].mem0)-1)<<4;

#ifdef DEBUG_
printf("len=#%08lX\n",len);
#endif

if ((void far *)(mem0=calloc(len,1))==NULL)
  {
  printf("fork: No memory\n");
  process_table[child_pos].pid=0;
  return -1;
  }

while (sema_lock(&sema_ps)==1);
process_table[child_pos].mem0=mem0;
process_table[child_pos].control_point[0].j_sp=user_sp+24;
process_table[child_pos].control_point[0].j_ss=FP_SEG(mem0);
process_table[child_pos].control_point[0].j_flag=
*(unsigned far*)MK_FP(user_ss,user_sp+22);
process_table[child_pos].control_point[0].j_cs=FP_SEG(mem0);
process_table[child_pos].control_point[0].j_ip=
*(unsigned far*)MK_FP(user_ss,user_sp+18);
process_table[child_pos].control_point[0].j_bp=
*(unsigned far*)MK_FP(user_ss,user_sp+ 0);
process_table[child_pos].control_point[0].j_di=
*(unsigned far*)MK_FP(user_ss,user_sp+ 2);
process_table[child_pos].control_point[0].j_es=
*(unsigned far*)MK_FP(user_ss,user_sp+ 8);
process_table[child_pos].control_point[0].j_si=
*(unsigned far*)MK_FP(user_ss,user_sp+ 4);
process_table[child_pos].control_point[0].j_ds=FP_SEG(mem0);
process_table[child_pos].control_point[0].j_ax=0;
process_table[child_pos].control_point[0].j_bx=
*(unsigned far*)MK_FP(user_ss,user_sp+14);
process_table[child_pos].control_point[0].j_cx=
*(unsigned far*)MK_FP(user_ss,user_sp+12);
process_table[child_pos].control_point[0].j_dx=
*(unsigned far*)MK_FP(user_ss,user_sp+10);
sema_unlock(&sema_ps);

/* переписываем туда процесс-родитель */
memcpy((void far *)mem0, (void far *)(process_table[parent_pos].mem0), len);

while (sema_lock(&sema_ps)==1);
i=process_table[child_pos].pid;
process_table[parent_pos].status=1; /* activate */
process_table[child_pos].status=1; /* activate */
sema_unlock(&sema_ps);
return i;
}
/****************************************************************************/
int load_process(int process_pos, char far * file)
{
char huge *mem, huge *mem0;

#ifdef DEBUG
printf("load_process: process_pos %i\n",process_pos);
printf("load_process: `%Fs'\n",file);
#endif

if ( (void far *)(mem0=calloc(ComMemory<<4,1)) == NULL )
  {
  printf("load_process: No memory\n");
  return 1;
  }

#ifdef DEBUG
printf("load_process: mem0=%08lX\n",mem0);
printf("load_process: `%Fs'\n",file);
#endif

while (sema_lock(&sema_ps)==1);
process_table[process_pos].mem0=mem0;
mem=mem0+0x100;
sema_unlock(&sema_ps);

if (load_file(file,mem))
  {
  printf("load_process: can't load `%Fs'\n",file);
  return 1;
  }

while (sema_lock(&sema_ps)==1);
process_table[process_pos].control_point[0].j_sp=0xFFF0;
process_table[process_pos].control_point[0].j_ss=FP_SEG(mem0);
process_table[process_pos].control_point[0].j_flag=_FLAGS;
process_table[process_pos].control_point[0].j_cs=FP_SEG(mem0);
process_table[process_pos].control_point[0].j_ip=0x100;
process_table[process_pos].control_point[0].j_bp=0;
process_table[process_pos].control_point[0].j_di=0;
process_table[process_pos].control_point[0].j_es=FP_SEG(mem0);
process_table[process_pos].control_point[0].j_si=0;
process_table[process_pos].control_point[0].j_ds=FP_SEG(mem0);
process_table[process_pos].control_point[0].j_ax=0;
process_table[process_pos].control_point[0].j_bx=0;
process_table[process_pos].control_point[0].j_cx=0;
process_table[process_pos].control_point[0].j_dx=0;
process_table[process_pos].status=1; /* activate */
sema_unlock(&sema_ps);
return 0;
}
/****************************************************************************/
int create_process(char far * file)
{int process_pos, i;

if ((process_pos=new_position())==-1)
  {
  return 1;
  }

i=load_process(process_pos, file);
if (i!=0)
  {
  while (sema_lock(&sema_ps)==1);
  process_table[process_pos].pid=0;
  sema_unlock(&sema_ps);
  }
return i;
}
/****************************************************************************/
void next_process(void)
{int i;
/* Переход к следующему заданию */
while (sema_lock(&sema_ps)==1);
for (i=0;i<PROC_MAX;i++)
  {
  if (process_table[i].pid)
    {
    if (process_table[i].status==1)
      {
      current_pid=process_table[i].pid;
      sema_unlock(&sema_ps);
      longjmp(process_table[0].control_point,0);
      }
    }
  }
puts("next_process: no processes");
L: goto L;
}
/****************************************************************************/
int exec (char far *path)
{
int proc_pos;
char path_buf [PATH_MAX];
char huge *saved;

#ifdef DEBUG
printf("exec: path=`%Fs'\n",path);
#endif

strncpy(path_buf,path,PATH_MAX);

#ifdef DEBUG
printf("exec: path_buf=`%s'\n",path_buf);
#endif

/*** 1. Тормозим текущий процесс */

while (sema_lock(&sema_ps)==1);
proc_pos=process_position(current_pid);
process_table[proc_pos].status=0;
sema_unlock(&sema_ps);

saved=process_table[proc_pos].mem0;

/*** 2. Создаем новый процесс */
if (load_process(proc_pos, path_buf))
  {
  printf("exec: can't load process\n");
  /* если загрузить новый процесс не удалось, продолжаем старый */
  while (sema_lock(&sema_ps)==1);
  process_table[proc_pos].status=1;
  process_table[proc_pos].mem0=saved;
  sema_unlock(&sema_ps);
  return 1;
  }

free((void far *)saved);

/* Переход к следующему заданию */
#ifdef DEBUG
#endif
putch('X');
next_process();
return 1999; /* сюда никогда не выйти (см. next_process() ) */
}
/****************************************************************************/
void start (char *cmd) /* start multitask */
{int i;

if (*cmd==0) {printf("usage: start <command> or s <command>\n"); return; }
chdir("/bin");
printf("command=%s\n",cmd);
Multitask=1;
create_process(cmd);
/* create_process("sh"); */
/* create_process("p1"); */
/* create_process("p2"); */
/* create_process("p3"); */

for (i=0;i<PROC_MAX;i++)
  {
  if (process_table[i].pid)
    {
    current_pid=process_table[i].pid;
    setvect(0x8,Int8); /* start scheduler on timer */
    longjmp(process_table[0].control_point,0);
    }
  }
puts("start: no processes");
}
/****************************************************************************/
void interrupt Int8 (void) /* Шедулер (планировщик), вызывается по таймеру */
{
int old_x=0,old_y=0;
int i, current_pos;
/*
Внимание! Это очень тонкое место. При изменении числа локальных
переменных в данной подпрограмме изменится ее ассемблерный код
(начало этого кода дано ниже в комментариях). При этом может
измениться константа в команде sub sp,4, помеченной двумя диезами #
в комментарии. При изменении константы нужно менять константу в
команде asm add word ptr DGROUP:_int8_point,28, помеченной
тремя диезами. При увеличении константы в sub на N нужно увеличивать
константу в add на столько же... Если же в данной процедуре вообще не
будет локальных переменных, то в ассемблерном коде исчезнет команда
mov bp,sp, помеченная одним диезом. Тогда эту команду нужно будет
указать явным образом (вместо ее закоментаренного варианта),
иначе дальнейшее базирование через регистр BP не будет работать...
А самое разумное, не трогать эту уже работающую подпрограмму и не
увеличивать и не уменьшать числа локальных переменных, а использовать
только глобальные и уже имеющиеся локальные... Но и здесь могут быть
подводные камни, так как компилятор может где-нибудь соптимизировать
(хотя я запретил оптимизацию) и при любом изменении кода нужно быть
готовым к глюкам и постоянно посматривать на ассемблерный вид этой
подпрограммы. Глюки будут скорее всего выражаться в виде повисания
системы (так как при неправильном значении констант SP будет уменьшаться
при каждом прерывании таймера (и вызове шедулера Int8()) и стек
наползет на код ядра (при увеличении SP будет в принципе то же самое,
так как адресация в пределах сегмента циклична)). */

/*        pushf                 */ /* old fl = [BP+22] */
/*        push cs               */ /* old cs = [BP+20] */
/*        push ip               */ /* old ip = [BP+18] */

/*        push    ax            */ /* old ax = [BP+16] */
/*        push    bx            */ /* old bx = [BP+14] */
/*        push    cx            */ /* old cx = [BP+12] */
/*        push    dx            */ /* old dx = [BP+10] */
/*        push    es            */ /* old es = [BP+ 8] */
/*        push    ds            */ /* old ds = [BP+ 6] */
/*        push    si            */ /* old si = [BP+ 4] */
/*        push    di            */ /* old di = [BP+ 2] */
/*        push    bp            */ /* old BP = [BP]    */
/*        mov     bp,DGROUP     */
/*        mov     ds,bp         */
/*        mov     bp,sp         */ /* # */
/*        sub     sp,4          */ /* ## */
/* [BP]=old BP */
/* #### */
asm     mov     word ptr DGROUP:_int8_point   ,SP; /* j_sp; */
asm     add     word ptr DGROUP:_int8_point   ,28; /* j_sp; */ /* ### */
asm     mov     word ptr DGROUP:_int8_point+ 2,SS; /* j_ss; */
asm     mov     ax,[BP+22];
asm     mov     word ptr DGROUP:_int8_point+ 4,ax; /* j_flag; */
asm     mov     ax,[BP+20];
asm     mov     word ptr DGROUP:_int8_point+ 6,ax; /* j_cs; */
asm     mov     ax,[BP+18];
asm     mov     word ptr DGROUP:_int8_point+ 8,ax; /* j_ip; */
asm     mov     ax,[BP+00];
asm     mov     word ptr DGROUP:_int8_point+10,ax; /* j_bp; */
asm     mov     ax,[BP+ 2];
asm     mov     word ptr DGROUP:_int8_point+12,ax; /* j_di; */
asm     mov     ax,[BP+ 8];
asm     mov     word ptr DGROUP:_int8_point+14,ax; /* j_es; */
asm     mov     ax,[BP+ 4];
asm     mov     word ptr DGROUP:_int8_point+16,ax; /* j_si; */
asm     mov     ax,[BP+ 6];
asm     mov     word ptr DGROUP:_int8_point+18,ax; /* j_ds; */
asm     mov     ax,[BP+16];
asm     mov     word ptr DGROUP:_int8_point+20,ax; /* j_ax; */
asm     mov     ax,[BP+14];
asm     mov     word ptr DGROUP:_int8_point+22,ax; /* j_bx; */
asm     mov     ax,[BP+12];
asm     mov     word ptr DGROUP:_int8_point+24,ax; /* j_cx; */
asm     mov     ax,[BP+10];
asm     mov     word ptr DGROUP:_int8_point+26,ax; /* j_dx; */

OldTimerVec();
outportb(0x20,0x20);

if (int8_point[0].j_cs==0xC000U) goto L_retn; /* работа с винтом нереентерабельна и если мы в BIOSе винта, то не переключаемся */
/* if (int8_point[0].j_cs==0xF000U) goto L_retn; */
/* if (int8_point[0].j_cs==0x0060U) goto L_retn; */
#if 0
if (Mode==0) goto L_retn; /* если мы в ядре, то не переключаемся */
#endif

#if 0 /* 1 - вывод чего-то в углу, 0 - нет (см. примечание в начале процедуры) */
old_x=wherex();
old_y=wherey();
gotoxy(70,0);
asm     mov     ax,SP;
asm     extrn   ohw:near;
asm     call    ohw;
gotoxy(old_x,old_y);
#else /* использование old_x, old_y, чтобы константа стека не изменилась */
  asm     mov     ax,old_x; /* см. примеч. в начале подпрограммы */
  asm     mov     ax,old_y;
#endif

/* while (sema_lock(&sema_ps)==1); */
if (sema_lock(&sema_ps)==1) goto L_retn;

if (current_pid)
  {
  /* запоминаем контрольную точку текущего процесса */
  current_pos=process_position(current_pid);
  process_table[current_pos].control_point[0].j_sp  =int8_point[0].j_sp;
  process_table[current_pos].control_point[0].j_ss  =int8_point[0].j_ss;
  process_table[current_pos].control_point[0].j_flag=int8_point[0].j_flag;
  process_table[current_pos].control_point[0].j_cs  =int8_point[0].j_cs;
  process_table[current_pos].control_point[0].j_ip  =int8_point[0].j_ip;
  process_table[current_pos].control_point[0].j_bp  =int8_point[0].j_bp;
  process_table[current_pos].control_point[0].j_di  =int8_point[0].j_di;
  process_table[current_pos].control_point[0].j_es  =int8_point[0].j_es;
  process_table[current_pos].control_point[0].j_si  =int8_point[0].j_si;
  process_table[current_pos].control_point[0].j_ds  =int8_point[0].j_ds;
  process_table[current_pos].control_point[0].j_ax  =int8_point[0].j_ax;
  process_table[current_pos].control_point[0].j_bx  =int8_point[0].j_bx;
  process_table[current_pos].control_point[0].j_cx  =int8_point[0].j_cx;
  process_table[current_pos].control_point[0].j_dx  =int8_point[0].j_dx;
  }
else
  {
  #if 1
  current_pos=0;
  #else
    printf("System crash 2");
    goto L_loop;
  #endif
  }

/* поиск следующего готового процесса */

i=current_pos;

while (1)
  {
  i++; if (i==PROC_MAX) i=0;
  if (process_table[i].pid) if (process_table[i].status==1)
    {
    current_pid=process_table[i].pid;
    sema_unlock(&sema_ps);
    longjmp(process_table[i].control_point,0);
    }
  if (i==current_pos)
    {
    /* ни одного готового к выполнению процесса, это редкое, но нормальное
    состояние (например, процесс 1 (shell) отфоркался и ждет запуска сына,
    а сын еще загружается с диска) */
    goto L_retn;
    }
  }
L_retn: ;
sema_unlock(&sema_ps);
}
/****************************************************************************/
void exit (int proc_pos)
{int i, j;
/*** Завершаем текущий процесс */

if (Multitask==0)

  {
  longjmp(the_return,0);
  }

while (sema_lock(&sema_ps)==1);

#ifdef DEBUG
printf("exit: current_pid=%i\n",current_pid);
#endif
proc_pos=process_position(current_pid);
#ifdef DEBUG
printf("exit: proc_pos=%i\n",proc_pos);
printf("exit: wait=%i\n",process_table[proc_pos].wait);
#endif
if ((j=process_table[proc_pos].wait)!=0) /* если окончания этого процесса кто-то */
  {i=process_position(j);                /* ждет - активируем его */
  #ifdef DEBUG
  printf("exit: activate pos. %i\n",i);
  #endif
  if (i!=-1)
    {
    process_table[i].status=1;
    #ifdef DEBUG
    printf("exit: activate ok\n");
    #endif
    }
  }
process_table[proc_pos].status=0;
process_table[proc_pos].pid=0;
sema_unlock(&sema_ps);

free((void far *)process_table[proc_pos].mem0);
#ifdef DEBUG
putch('e');
#endif
next_process();
}
/****************************************************************************/
int wait (pid_t pid)
{int i, j;
/* Если процесс pid отсуствует, то return 0 */
while (sema_lock(&sema_ps)==1);
#ifdef DEBUG
printf("wait(%i);\n",pid);
#endif
i=process_position(pid);
#ifdef DEBUG
printf("wait: his pos.[%i]\n",i);
#endif
if (i==-1)
  {
  sema_unlock(&sema_ps);
  #ifdef DEBUG
  printf("wait: no pid\n");
  #endif
  return 0;
  }
/* Делаем текущий процесс неактивным и помечаем тот процесс, кого ждем */
j=process_position(current_pid);
#ifdef DEBUG
printf("wait: cur. pos. %i\n",j);
#endif
if (j!=-1) process_table[j].status=0;
process_table[i].wait=current_pid;
sema_unlock(&sema_ps);

/* цикл ожидания */
while (1)
  {
  while (sema_lock(&sema_ps)==1);
  if (process_table[j].status==1) break;
  sema_unlock(&sema_ps);
  }
sema_unlock(&sema_ps);
#ifdef DEBUG
putch('w');
#endif
return 0;
}
/***********************************END**************************************/