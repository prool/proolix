// C-headers for c utilities

#define NULL 0
typedef unsigned int size_t;

void stop(void);
void putch(char c);
void putch2(char c);
void putch3(char c);
void puts0(char *s);
void putch_color(char c, char attrib);
void puthex(int c);
int getch(void);
char *getsn(char *str, int len);
size_t strlen (const char *s);
char *strchr (const char *str, int c);
char  * strncpy (char  * dest, const char  * src, size_t maxlen);

char get_row(void);
char get_col(void);
void setpos (char row, char col);

char get_color(void);
void set_color(char color);

void help(void);
void test(void);
void ascii(void);
void cls(void);

char  *itoa (int w, char  *str, int radix);

void readboot (char *buffer);
void out_boot(void *buf);

struct BootStru /* structure of boot sector of FAT12 and FAT16*/
  {
  unsigned char Jmp [3];
  unsigned char OEM [8];
  int  SectSiz;
  char ClustSiz;
  int  ResSecs;
  char FatCnt;
  int  RootSiz;
  unsigned int  TotSecs;
  unsigned char Media;
  int  FatSize;
  int  TrkSecs;
  int  HeadCnt;
  unsigned long HidnSec;
  unsigned long BigSect;
  unsigned char DriveNo;
  unsigned char Thing;
  unsigned char BootSign;
  int  SerialNo [2];
  unsigned char VolLbl [11];
  unsigned char FileSysId [8];
  /*
  int  FATaddr;
  int  StartClu;
  int  RootSize;
  */
  };
