// view exe (PE EXE) header. built under cygwin/64. prool

// info about PE EXE format: http://mzc.narod.ru/Creating/Step008.htm

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#define BUFLEN 1024


//#include "..\include\limits.h"

struct reloc
  {
  unsigned int offset;
  unsigned int segment;
  };

struct pe_exe_header2
{
char PE[2]; // magic 'PE'
char Zeores[2]; // two zeroes ;)
unsigned short int CPUType;
unsigned short int SectionCount;
unsigned int DateTime;
};

/* заголовок исполняемого EXE MZ файла (как в MSDOS) */
struct pe_exe_header
{                     /* комментарии из [TECH Help!] */
char MZ[2];           /* "подпись" файла .EXE ('MZ') */
unsigned short int PartPag; /* длина неполной последней страницы (обычно игнорируется) */
unsigned short int PageCnt; /* длина образа в 512-байтовых страницах, включая заголовок */
unsigned short int ReloCnt; /* число элементов в таблице перемещения */
unsigned short int HdrSize; /* длина заголовка в 16-байтовых параграфах */
unsigned short int MinMem;  /* минимум требуемой памяти за концом программы (параграфы) */
unsigned short int MaxMem;  /* максимум требуемой памяти за концом программы (параграфы) */
unsigned short int ReloSS;  /* сегментное смещение сегмента стека (для установки SS) */
unsigned short int ExeSP;   /* значение регистра SP (указателя стека) при запуске */
unsigned short int ChkSum;  /* контрольная сумма (отрицательная сумма всех слов в файле) */
unsigned short int ExeIP;   /* значение регистра IP (указателя команд) при запуске */
unsigned short int ReloCS;  /* сегментное смещение кодового сегмента (для установки CS) */
unsigned short int TablOff; /* смещение в файле 1-го элемента перемещения (часто 001cH) */
unsigned short int Overlay; /* номер оверлея (0 для главного модуля) */
// end of old structure - EXE MZ
// begin of new structure - PE EXE
unsigned short int Reserved[4]; // reserved
unsigned short int OEMId; // For OEMInfo
unsigned short int OEMInfo;
unsigned short int Reserved2[10]; // reserved
unsigned short int PEHeaderAddr;

//struct reloc RelTbl [];
};

void ViewPE (struct pe_exe_header2 *Buf)
{
time_t mytime;
printf("PE magic %c%c\n", Buf->PE[0], Buf->PE[1]);

printf("CPUType %04X\n",Buf->CPUType);
printf("SectionCount %04X\n",Buf->SectionCount);
printf("DateTime %08X = ",Buf->DateTime);

mytime=Buf->DateTime;

printf("%s\n",(char *) asctime(localtime(&mytime)));

printf("...\n");
}

int ViewEXE (struct pe_exe_header  *Buf)
{long i, j;
printf("Bytes on last page %04X\n",Buf->PartPag );
printf("Pages in file %04X\n",Buf->PageCnt );
printf("Relocations %04X\n",Buf->ReloCnt );
printf("Paragraph in header %04X\n",Buf->HdrSize );
printf("MinMem %04X par\n",Buf->MinMem  );
printf("MaxMem %04X par\n",Buf->MaxMem  );
printf("SS:SP %04X:%04X\n",Buf->ReloSS,Buf->ExeSP);
printf("CheckSum %04X\n",Buf->ChkSum  );
printf("CS:IP %04X:%04X\n",Buf->ReloCS,Buf->ExeIP);
printf("Relocation table addr %04X\n",Buf->TablOff );
printf("Overlay No. %1X\n",Buf->Overlay);
printf("Reserved:"); for (i=0;i<4;i++) printf(" %04X", Buf->Reserved[i]); printf("\n");
printf("OEMId %04X\n",Buf->OEMId);
printf("OEMInfo %04X\n",Buf->OEMInfo);
printf("Reserved#2:"); for (i=0;i<10;i++) printf(" %04X", Buf->Reserved2[i]); printf("\n");
printf("PEHeaderAddr %04X\n",i=(Buf->PEHeaderAddr));
return i;
}
/* Из [TECH Help!]

г============================================================================¬
¦                        Структура заголовка файла EXE                       ¦
L============================================================================-

Смещ.  Длина Содержимое
------ ----- -----------------------------------------------------------------
            --------¬
 +0      2  ¦4Dh 5aH¦ "подпись" файла .EXE ('MZ')
            +---+---+
 +2      2  ¦PartPag¦ длина неполной последней страницы (обычно игнорируется)
            +---+---+
 +4      2  ¦PageCnt¦ длина образа в 512-байтовых страницах, включая заголовок
            +---+---+
 +6      2  ¦ReloCnt¦ число элементов в таблице перемещения
            +---+---+
 +8      2  ¦HdrSize¦ длина заголовка в 16-байтовых параграфах
            +---+---+
+0aH     2  ¦MinMem ¦ минимум требуемой памяти за концом программы (параграфы)
            +---+---+
+0cH     2  ¦MaxMem ¦ максимум требуемой памяти за концом программы (параграфы)
            +---+---+
+0eH     2  ¦ReloSS ¦ сегментное смещение сегмента стека (для установки SS)
            +---+---+
+10H     2  ¦ExeSP  ¦ значение регистра SP (указателя стека) при запуске
            +---+---+
+12H     2  ¦ChkSum ¦ контрольная сумма (отрицательная сумма всех слов в файле)
            +---+---+
+14H     2  ¦ExeIP  ¦ значение регистра IP (указателя команд) при запуске
            +---+---+
+16H     2  ¦ReloCS ¦ сегментное смещение кодового сегмента (для установки CS)
            +---+---+
+18H     2  ¦TablOff¦ смещение в файле 1-го элемента перемещения (часто 001cH)
            +---+---+
+1aH     2  ¦Overlay¦ номер оверлея (0 для главного модуля)
            L---+----
1cH         размер форматированной порции заголовка EXE
            --------T-------T - - T-------T-------¬ Таблица перемещения. Начало
+ ?     4*? ¦ смещ.  сегмент¦  ¦ смещ.  сегмент¦ по смещению [EXE+18H]. Имеет
            L---+---+---+---+ - - +---+---+---+---- [EXE+6] 4-байтовых элемента.
+ ?     ?   пропуск до границы параграфа
+ ?     ?   начало образа программы
--------------------------------------------------------------------------------
Поскольку EXE-файл может быть загружен в любой сегмент, все абсолютные сегмент-
ные ссылки (FAR CALL, далекие указатели, ссылки типа MOV AX,data_seg) должны
быть приведены к адресам памяти, соответствующим загрузке. Ниже приведены шаги,
используемые программой загрузки DOS (функция 4bH ) при загрузке файла EXE:

1.  создать PSP посредством функции DOS 26H

2.  прочитать 1cH байт файла EXE (форматированную порцию заголовка EXE)
    в локальную область памяти

3.  определить размер модуля = ( (PageCnt*512)-(HdrSize*16) ) - PartPag

4.  определить файловое смещение загружаемого модуля = (HdrSize * 16)

5.  выбрать сегментный адрес, START_SEG, для загрузки (обычно  PSP + 10H)

6.  считать модуль в область памяти, начинающуюся с адреса START_SEG:0000

7.  LSEEK (уст. указатель файла) на начало таблицы перемещения (TablOff)

8.  для каждого элемента перемещения (ReloCnt):
    a. считать элемент как два 16-битовых слова (I_OFF,I_SEG)
    b. вычислить RELO_SEG=(START_SEG+I_SEG) (адрес перемещаемой ссылки)
    c. извлечь слово по адресу RELO_SEG:I_OFF (прочитать текущее значение)
    d. прибавить START_SEG к этому слову (выполнить привязку сегмента)
    e. поместить результат обратно по его адресу (RELO_SEG:I_OFF)

9.  Распределить память для программы согласно MaxMem и MinMem

10. Инициализировать регистры и запустить программу:
    a. ES = DS = PSP
    b. AX = отражает корректность идентификаторов дисков в командной строке
    c. SS = START_SEG+ReloSS, SP = ExeSP
    d. CS = START_SEG+ReloCS, IP = ExeIP
    (команды: PUSH seg; PUSH offset; RETF)
 */

void ident (void)
{
printf("View EXE (PE EXE) ver. 0.0.1.3 by Prool, 17-aug-2015, cygwin/64\n\n");
}
/////////////////////////////////////////////// MAIN //////////////////////////////////////////////////
void main (int argc, char *argv[])

{char buf [BUFLEN]; int h, i;
int pe_start;

#if 1
printf("sizeof int = %i\n", sizeof (int));
printf("sizeof short int = %i\n", sizeof (short int));
#endif

if (argc!=2)
  {
  ident();
  printf("usage: vexe filename");
  }
else
  {int len;
  if ((h=open(argv[1],O_RDONLY | O_BINARY))==-1)
    printf("Can't open %s\n",argv[1]);

  len = sizeof (struct pe_exe_header);
  if ((i=read(h,buf,len))!=len)
    printf("Can't read %s. read()=%i\n",argv[1],i);
  printf("File %s\n\n",argv[1]);
  pe_start=ViewEXE((void *)buf);
  printf("\n");

  if (lseek(h,pe_start,SEEK_SET)==-1) {printf("lseek error\n"); return;}
  len=sizeof(struct pe_exe_header2);

  if ((i=read(h,buf,len))!=len)
    printf("Can't read %s. read()=%i\n",argv[1],i);

  ViewPE((void *)buf);

  ident();
  close(h);
  }
}
