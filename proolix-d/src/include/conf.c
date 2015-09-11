/*      conf.c

        Configuration file for Proolix

        Serge Pustovoitoff 1994-2004

*/

/* Определение имени DOSKEY подключает написанный на ассемблере shell Сазонова
с редактированием командной строки, отсуствие онного - ортодоксальный шелл,
написанный мной на С (без редактирования, только с клавишей Break */
/* #define DOSKEY */

/* Аппаратура */

/* Если определено CMOS - значит, в машине есть CMOS и RTC - Real Time Clock
(AT и старше, хотя бывают и XT с CMOS RTC ) */
#define CMOS

/* Если DEBUG определено, то будет выдаваться отладочная информация */
/* #define DEBUG */

/* Если _RC_ определено, то при загрузке Пруликса будет выполняться файл
_RC_ */
#define _RC_ "rc.com"

/* перехват кучи прерываний - глючит (иначе оставляем значение вектора от
ROM BIOS */
/* #define CHANGE_INTS */

/* перехват int 5 - глючит (иначе оставляем значение вектора от ROM BIOS */
/* #define CHANGE_INT5 */

/* перехват int 1B (ctrl-break) */
#define CHANGE_INT_1B

/* очистка экрана при загрузке Пруликса */
/* #define CLRSCR */

/* Видеокарта */
#define _VGA_ /*_DEFAULT_VIDEO_ _MDA_ _HERCULES_ _CGA_ _EGA_ _VGA_ */
#ifdef _HERCULES_
        /* #define _MSHERC_ */ /* Это виснет */
#endif
/* Видеорежим */
#define _VIDEO_ASCII_ /* _VIDEO_ASCII_ _VIDEO_GRAPH_ */
/* Монитор цветной/монохромный */
#define _MON_MONO_ /* _MON_COL_ _MON_MONO_ */
/* Адрес видео сегмента*/
#define VIDEO_SEG 0xB800 /* 0xB000 0xB800 */
/* Main text char attribute */
#define MAIN_TEXT_ATTR 7

/* putch */
#define PutchBIOS putch0 /* putch0 putch1 putch2 putch3 */
/* putch0 - asm-program, use int 10h Fn=E (emulate tty)
   putch1 - asm-program, use int 10h Fn=9 (put sym & attr)
   putch2 - asm-program (direct write to video RAM). putch2 maked from putch3
     (compiled from C to asm manually (like manual sunset ;))
     Debugged under MSDOS (into HCAT2 program). Worked under MSDOS.
     Not worked under Proolix
   putch3 - C-program (direct write to video RAM). Debugged under MSDOS
     (into HCAT program). Worked under MSDOS. Not worked under Proolix :(
 */

#if defined(_HERCULES_) && defined(_VIDEO_GRAPH_)
  #ifdef PutchBIOS
  #undef PutchBIOS
  #endif
#define PutchBIOS putch1
#endif

/* Proolix System Call Interrupt Number */
/* MSDOS have number 21h ! See [Interrupt List] for search free & unusable
interrupt no */
/* #define INT_NO 0x83 */ /* Old syscall */
#define INT_NO 0x84 /* New syscall */

/* Use processor definition library by B-coolWare.
Владимир Захарычев 2:5028/52.6 bob@ymz.yaroslavl.su */

/* #define B_COOLWARE */

#define HDD_READ_ONLY 0 /* 0 - HDD mounted read/write,
                           1 - HDD mounted read only */
#define LASTDRIVE 16 /* max drives */

#define _STANDARD_ 0
#define _RUSSIAN_ 1

#define CODETABLE _RUSSIAN_

#define FONT_DIR "/bin"
#define HOME_DIR "/"
