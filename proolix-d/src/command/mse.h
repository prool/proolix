#define ENV_SIZE 300
#define MAX_COMMAND_STRING 120
#define ARGV0LEN 13
#define STRLEN 100
#define DOS_MAJOR 3
#define DOS_MINOR 20
#define EMULATOR_COLOR 12
#define DOS_COLOR 7
#define INTERRUPT_TABLE_SIZE 256*4

#include <setjmp.h>

void interrupt Int20(void);
void interrupt Int21(void);
void interrupt Int23(void);
void interrupt Int24(void);
void interrupt Int29(void);

struct DTAfind
  {
  int dir_handle;
  char reserv [0x13];
  char attr;
  int time;
  int date;
  long size;
  char filename [13];
  };

struct country_structure
  {
  int date_time;   /* 0 = month day year,  hh:mm:ss  (USA)
                      1 = day month year,  hh:mm:ss  (Europe)
                      2 = year month day,  hh:mm:ss  (Japan) */
  char currency [5];
  char thousands_sep [2];
  char decimal_sep [2];
  char date_sep [2];
  char time_sep [2];
  char currency_lead;     /* 0 = symbol leads, without space
                             1 = symbol follows, without space
                             2 = symbol leads, one space
                             3 = symbol follows, one space
                             4 = symbol replace decimal separator */
  char digital_after_dec;
  char time_format; /* Bit 0   = 0  12 hour clock
                                   = 1  24 hour clock */
  long case_map_call;
  char asciiz_sep;
  char Reserved [10];
  };

extern struct country_structure Ukraine;
extern char far * pEnv;
extern char allow_dos_functions [255];
extern char verbose_dos_functions [255];
extern char allow_change_int [255];
extern int msdos_verbose, msdos_pause;
extern jmp_buf return_point;
extern char huge *aPSP;
extern char osmajor, osminor, osbreak;
extern struct DTAfind far * DTAaddr;
extern int TSR;
