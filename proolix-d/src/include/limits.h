/*      limits.h

        Defines implementation specific limits on type values.

        Copyright (c) Serge Pustovoitoff 1993-1998

*/

#ifndef LIMITS_H

#define LIMITS_H here

/* Количественные ограничения в Пруликсе. В скобках - минимальное значение
согласно стандарту POSIX */

#define ARG_MAX 120  /* (4096) макс. длина списка аргументов и среды */
#define MAX_ENV 20    /* максимальное число переменных в среде */
#define CHAR_BIT 8    /* (8) макс число бит в данных типа char */

/* максимальное и минимальное знач данных типа         char */
#if (((int)((char)0x80)) < 0)
#define CHAR_MAX                        0x7F
#define CHAR_MIN                        0x80
#else
#define CHAR_MAX                        0xFFU
#define CHAR_MIN                        0x00
#endif

#define CHILD_MAX 6 /* (6) макс число одновр сущ процессов с одинак euid */
#define CLK_TCK  18 /* (10) число тиков в секунде */
        /* On TurboC 2.0 #define CLK_TCK 18.2 */
#define INT_MAX       0x7FFF  /* ( 32767) */
#define INT_MIN ((int)0x8000) /* (-32767) */
#define LINK_MAX 8 /* (8) макс число связей файла */
#define LONG_MAX        0x7FFFFFFFL /*  ( 2147483647)  */
#define LONG_MIN ((long)0x80000000L) /* (-2147483647)  */
#define MAX_CANON 255 /* (255) макс число байт в канонич очереди терминала */
#define MAX_INPUT 255 /* (255) */
#define MB_CUR_MAX 2 /* (MB_LEN_MAX) */
#define MB_LEN_MAX 2 /* (1) */
#define NAME_MAX 14 /* (14) макс число байт в имени файла, не считая
                            завершающего NUL */
#define NGROUPS_MAX 1 /* (0) */

#ifndef OPEN_MAX
#define OPEN_MAX 8 /* (16) макс число одновр открытых файлов */
#endif

#ifndef TMP_MAX
#define TMP_MAX 25 /* (25) */
#endif

#define PATH_MAX 120 /* (255) макс длина пути */
#define PIPE_BUF 512 /* (512) */
#define SCHAR_MAX 0x7F /* макс знач данных типа signed char */
#define SCHAR_MIN -128 /* мин. знач данных типа signed char */
#define SHRT_MAX       0x7FFF  /* макс знач данных типа short */
#define SHRT_MIN ((int)0x8000) /* мин. знач данных типа short */
#define UCHAR_MAX 0xFFU /* макс знач данных типа unsigned char */
#define UINT_MAX  0xFFFFU      /* (     65535) */
#define ULONG_MAX 0xFFFFFFFFUL /* (4294967295) */

#define USHRT_MAX 0xFFFFU /* (65535) */

#define PROC_MAX 4 /* максимальное число процессов */

/* Мои собственные ограничения: */

/* Пруль - ограниченный человек ;) */

/* максимальная длина строки */
#define MAX_LEN_STR 250

#define PassLen 80
#define MAX_ARG 10
#define SECTOR_SIZE 512
#define FAT_SECS 7
#define MAX_LINE 24
#define RETRYES 15
#define BUFFERS 5 /* max 127. Макс. размер буферного пула - 64К, длина
буфера 512+6=518 б, 64К % 518 = 127. */
/* Память, выделяемая каждому COM-файлу (параграфов) */
#define ComMemory 0x1000UL

#endif