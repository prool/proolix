// C-headers for c utilities

// variables

extern int SectorsOnCyl;
extern int TrkSecs;
extern int HeadCnt;
extern int RootBeg;
extern int DataStart;
extern int MaxSectors;
extern int ResSecs;
extern int CluSize;
extern int CluSizeBytes;
extern int FatSize;
extern int RootEnd;
extern int MaxClusters;

#define NULL 0
typedef unsigned int size_t;

void printf(...); // proolfool

void stop(void);
void putch(char c);
void putch2(char c);
void putch3(char c);
void puts0(char *s);
void putch_color(char c, char attrib);
void puthex(int c);
void puthex_l(int c);
void putdec(int);
void putdec2(int, int, int);
int getch(void);
char *getsn(char *str, int len);
size_t strlen (const char *s);
char *strchr (const char *str, int c);
char  * strncpy (char  * dest, const char  * src, size_t maxlen);

int peek (int addr);

char get_row(void);
char get_col(void);
void setpos (char row, char col);

char get_color(void);
void set_color(char color);

void help(void);
void memmap(void);
void system(void);
void test(void);
void ascii(void);
void cls(void);
void memd(void);
void basic(void);
void diskd0(void);
void diskd(void);
void ls(void);

char  *itoa (int w, char  *str, int radix);

void readboot (char *buffer);
void out_boot(void *buf);
void process_boot(void *buf);
int readsec0(char drive, char sec, char head, char trk /* or cyl */, char *Buffer);

struct __attribute__((__packed__)) BootStru /* structure of boot sector of FAT12 and FAT16*/
{
/*************************************************************
Begin:
                jmp     short l
                db      90h
;                       '12345678'
OEM             db      'KERNEL',0,0
SectSiz         dw      512
ClustSiz        db      2
ResSecs         dw      1
FatCnt          db      2
RootSiz         dw      112
TotSecs         dw      720
Media           db      0fdh
FatSize         dw      2
TrkSecs         dw      9
HeadCnt         dw      2
HidnSec         dd      0
BigNo           dd      0       ; Big total no of sectors
                dw      0       ; Physical drive no
                db      9       ; Extended boot signature
                ;dd     1       ; Volume serial no
FATaddr         dw      0       ; ! internal vars
StartClu        dw      0       ; !
Lbl             db      'No_Kernel',0,0   ; Volume label
                db      'FAT12 ' ; File system ID
RootSize        dw      '  '; use 2 last bytes of File system id
OldVec          dw      2 dup (0)
*************************************************************/
  unsigned char Jmp [3];
  unsigned char OEM [8];
  unsigned short int  SectSiz;
  unsigned char ClustSiz;
  unsigned short int  ResSecs;
  unsigned char FatCnt;
  unsigned short int  RootSiz;
  unsigned short int  TotSecs;
  unsigned char Media;
  unsigned short int  FatSize;
  unsigned short int  TrkSecs;
  unsigned short int  HeadCnt;
  unsigned int HidnSec;
  unsigned int BigSect;
  unsigned char DriveNo;
  unsigned char Thing;
  unsigned char BootSign;
  unsigned short int  SerialNo [2];
  unsigned char VolLbl [11];
  unsigned char FileSysId [8];
};

#define FP_OFF(fp)      ((unsigned)(fp))
#define FP_SEG(fp)      ((unsigned)((unsigned long)(fp) >> 16))

#define MK_FP(seg,ofs)  ((void *) \
                           (((unsigned long)(seg) << 16) | (unsigned)(ofs)))

struct __attribute__((__packed__)) dirent16 /* directory structure of FAT-12 and FAT-16 */
{
char d_name [11];
unsigned char AttrByte;
char Reserv [10];
unsigned short int FileTime;
unsigned short int FileDate;
unsigned short int d_fileno; /* initial cluster number */
unsigned int Size;
};
