/*
                                                "Процесс пошел"

                                                        М.С.Горбачев

 ****************************************************************************/
/* #define DEBUG_PROCESS */
/****************************************************************************/
extern char sh_str_orig [];
jmp_buf the_return;
#define F_COM 0
#define F_EXE 1
#define F_ERR 2
#define I_OFF Twix[0]
#define I_SEG Twix[1]
/****************************************************************************/
jmp_buf jmpb;
unsigned int Twix[2];
/****************************************************************************/
int execve (char far *name, char far *_argv[], char far *far *env)
{
char Sec1 [SECTOR_SIZE];
char FirstSec [SECTOR_SIZE];
int i, h, argc, nenv, restos, segment, offset_s;
long int FileLen, l;
unsigned int len;
size_t SizeMemoryBlock;
char far *cc, huge *mem, huge *mem0=0, far *ptr, far *adr, far *EnvAdr;
int status, n_sec;
struct exe_header *H;
unsigned int exe_begin, RELO_SEG;

if ((h=open(name,O_RDONLY))==-1)
  {
  return 1;
  }

if ((FileLen=filelength(h))<4)
  {
  R4:
  close(h);
  return 2;
  }

/*** Чтение первого сектора и проверка на сигнатуру DEAD/DEAF ***/
restos = (SECTOR_SIZE>FileLen) ? (int)FileLen : SECTOR_SIZE;
if (read(h,Sec1,restos) != restos)
  {close(h); return 3; }

if (Sec1[0]==0xDE)
  {
  switch (Sec1[1])
    {
    case 0xAD: status=F_COM; break;
    case 0xAF: status=F_EXE; /* ViewEXE((void *)Sec1); */
               break;
    default  : status=F_ERR;
    }
  }
else status=F_ERR;

if (status==F_ERR) goto R4;

if (status==F_COM) SizeMemoryBlock= ComMemory<<4; /*((ComMemory -1U)<<4)+1;*/
if (status==F_EXE)
  { /* F_EXE */
  H=(void *)Sec1;
  SizeMemoryBlock=((long)(H->PageCnt)+1L)*SECTOR_SIZE+
    (long)(H->MinMem/*-H->HdrSize*/)*16;
  /* printf("exe mem size=%li\n",SizeMemoryBlock); */
  }
if ( (void far *)(mem0=calloc(SizeMemoryBlock,1)) == NULL )
  {
  printf("execve: No memory. (Required %l bytes)\n",SizeMemoryBlock);
  return 5;
  }
mem=mem0+0x100;
if (status==F_EXE)
  {
  exe_begin=FP_SEG(mem0)+0x10;
  /* printf("exe_begin=%04X\n",exe_begin); */
  /* printf("mem0=%08lX\n",mem0); */
  /* printf("mem=%08lX\n",mem); */
  }

/* Перезапись глобальных переменных stdin, stdout и stderr в PSP
(см. описание этих перем. в stdio.h и command/c1.asm */

*(FILE far * far *)(mem0+0x0A)=stdin;
*(FILE far * far *)(mem0+0x0E)=stdout;
*(FILE far * far *)(mem0+0x12)=stderr;

if (status==F_COM)
  {
  memcpy((void far *)mem,Sec1,restos);
  mem+=restos;
  n_sec=(int)(FileLen/SECTOR_SIZE );
  }
if (status==F_EXE)
  {
  n_sec=(H->PageCnt/2)+1-(H->HdrSize/(SECTOR_SIZE/16))+1;
  /* printf("n_sec=%i\n",n_sec); */
  }

/* Чтение остальных секторов */
/* F_COM, F_EXE */
for (l=1;l<n_sec;l++)
  {
  if (read(h,(void far *)mem,SECTOR_SIZE)!=SECTOR_SIZE) break;
  mem+=SECTOR_SIZE;
  }

if (status==F_COM) if (FileLen>SECTOR_SIZE)
  {
  restos=(int)(FileLen % SECTOR_SIZE);
  read(h,(void far *)mem,restos);
  }

#if 1
if (status==F_EXE)
  { /* Настройка перемещаемых адресов в exe-модуле */
  if (lseek(h,H->TablOff,SEEK_SET)==-1L)
    {free((void far *)mem0);close(h);return 5;}
  for (i=0;i<H->ReloCnt;i++)
    {
    if (read(h,(void *)Twix,4)!=4)
      {free((void far *)mem0);close(h);return 6;}
    /* printf("relocation %04X:%04X\n",I_SEG,I_OFF); */
    RELO_SEG=exe_begin+I_SEG;
    *(unsigned int far *)MK_FP(RELO_SEG,I_OFF) =
      *(unsigned int far *)MK_FP(RELO_SEG,I_OFF)+exe_begin;
    /* putch('@'); */
    }
  }
#endif

close (h);
puts("");

if (status==F_COM)
  {
  /* Перенос параметров argc, argv и env внутрь пользовательского процесса */
  len=0;
  for (argc=0;_argv[argc]!=NULL;argc++) len+=(int)(strlen(_argv[argc])+1);
  len+=(2+2+2*(argc));
  /*
  2         -  длина параметра argc (int - 2 байта)
  2         -  длина параметра argv (near ptr - 2 байта)
  2 * argc  -  длина параметра char *argv[]
  len       -  сумма длин строк-аргументов
  */
  nenv=0;  while(env[nenv]!=NULL) {len+=(int)(strlen(env[nenv++])+1);}
  len+=(2+2*(nenv+1));
  if (len & 1) len++; /* делаем len четным */

  ptr=(char far *)mem0+(unsigned int) 0xfffe-len;
  segment=FP_SEG(ptr);

  /*** Параметры функции main ***/
  /*** argc ***/
  *(int far *)ptr=argc; ptr+=2;

  /*** argv ***/
  *(int far *)ptr=FP_OFF(ptr+4); ptr+=2;

  /*** env ***/
  EnvAdr=ptr;
  ptr+=2;

  /* Перезапись argv в пользовательский процесс */

  offset_s=FP_OFF(ptr)+(argc)*2;

  for (i=0;i<argc;i++)
    {
    *(int far *)ptr=offset_s;
    ptr+=2;
    cc=_argv[i];
    do
      {
      adr=MK_FP(segment,offset_s++);
      *adr = *cc;
      }
    while (*cc++);
    }
  /* Перезапись env в пользовательский процесс */

  ptr=MK_FP(segment,offset_s);
  *(int far *)EnvAdr=offset_s;
  /* Перезапись адреса среды в extern переменную environ */
  *(int far *)(mem0+0x16)=offset_s;
  offset_s+=(2*(nenv+1));
  for (i=0; i<nenv; i++)
    {
    *(int far *)ptr=offset_s;
    ptr+=2;
    cc=*(env+i);
    do
      {
      adr=MK_FP(segment,offset_s++);
      *adr = *cc;
      }
    while (*cc++);
    }
  *(int far *)ptr=0;
  }

Mode=1;
User=1;
ProcessPar=FP_SEG(mem0);
asm   lea     ax,LL;
i=_AX;
setjmp(the_return);

the_return[0].j_ip=i;

if (status==F_COM) execpix((void far *)mem0,len);

if (status==F_EXE)
  {
  /* asm   int     3; */
  jmpb[0].j_cs=exe_begin+H->ReloCS;
  jmpb[0].j_ip=H->ExeIP;
  jmpb[0].j_sp=H->ExeSP;
  jmpb[0].j_ss=exe_begin+H->ReloSS;
  jmpb[0].j_flag=_FLAGS;
  jmpb[0].j_bp=0;
  jmpb[0].j_di=0;
  jmpb[0].j_es=FP_SEG(mem0);
  jmpb[0].j_si=0;
  jmpb[0].j_ds=FP_SEG(mem0);
  jmpb[0].j_ax=0;
  jmpb[0].j_bx=0;
  jmpb[0].j_cx=0;
  jmpb[0].j_dx=0;
  #ifdef DEBUG_PROCESS
  printf("Ll");
  #endif
  longjmp(jmpb,0);
  }

asm   LL      label   near;

/* После выхода закрываем открытые файлы */
for (i=3;i<OPEN_MAX;i++)if (FCBS[i].Name[0]) close(i);

Mode=0;
User=0;
ProcessPar=kernel_begin();
free((void far *)mem0);
return 0;
}