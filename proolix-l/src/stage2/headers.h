// C-headers for c utilities

#define NULL 0
#define SECTOR_SIZE	512

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define O_READ 0
#define O_WRITE 1
#define O_CREAT 2
#define O_APPEND 3

// variables

#if 0
extern unsigned char buffer512[];
extern unsigned short errno;

extern short int SectorsOnCyl;
extern short int TrkSecs;
extern short int HeadCnt;
extern short int RootBeg;
extern short int DataStart;
extern int MaxSectors;
extern int ResSecs;
extern short int CluSize;
extern short int CluSizeBytes;
extern short int FatSize;
extern short int RootEnd;
extern short int MaxClusters;
extern short int MaxCyl;

extern unsigned char current_drive; // 0 - FDD A, 1 - FDD B, 80 - HDD 1, 81 - HDD 2, FF - no mount drive
extern unsigned short int boot_drive;

extern int reg_ax;
extern int reg_bx;
extern int reg_cx;
extern int reg_dx;

extern int reg_si;
extern int reg_di;

extern int reg_es;
#endif

extern unsigned short int gCyl[4];
extern unsigned short int gSec[4];
extern unsigned short int gHeads[4];
extern unsigned short int gTotal[4];

extern unsigned short int FCB [6];
/*
	FCB[0]	directory block with file
	FCB[1]	number file record in directory block
	FCB[2]	current file block
	FCB[3]  |
	FCB[4]	| offset
	FCB[5]  file mode (O_APPEND, O_READ, O_CREATE)
	FCB[6]  |
	FCB[7]  | file length (исп. в реж. O_READ и возможно в будущих O_WRITE, O_RDWR)
*/

typedef unsigned int size_t;

void stop(void);
void putch(char c);
void putch_tty(char c);
void putch2(char c);
void putch3(char c);
void puts0(char *s);
int puts(char *s);
void putch_color(char c, char attrib);
void puthex(int c);
void puthex_l(int c);
void putdec(int);
void putdec2(int, int, int);
int getch(void);
int kbhit(void);
char *getsn(char *str, int len);
size_t strlen (const char *s);
char *strchr (const char *str, int c);
char  * strncpy (char  * dest, const char  * src, size_t maxlen);
int strcmp (const char  *s1, const char  *s2);

int peek (int addr);
short int peek2(short int seg, short int offset);
void poke(char value, short int seg, short int offset);
unsigned short int end_of (void);
unsigned short int get_sp (void);
short int get_rtc(void);
short int get_day(void);
short int get_year(void);
void print_boot(void);

void reboot (void);
void cold (void);
void hdd0 (void);
void hdd1 (void);
void fdd (void);

char get_row(void);
char get_col(void);
void setpos (char row, char col);

char get_color(void);
void set_color(char color);

void gluck(void);
void gluck2(char *);

void help(void);
void memmap(void);
void system(void);
void test(void);
void cat(void);
void catw(void);
void ascii(void);
void cls(void);
void cls_(void);
void scroll(void);
void off(void);
void memd0(void);
void memd(void);
void basic(void);
void diskd0(void);
void diskd(void);
void testdisk(void);
void mount(void);
void out_os(unsigned char);
void out_fat(void);
void skript(void);
void vectors(void);
void screensaver(void);
void install(void);
void format(void);
void view_superblock(void);
void ls(void);
void create_file(void);
void create_file2(void);
void remove_file(void);
int open_ (char *filename, int flag);
int close_(int h);
int writec(int h, char c);
int readc (int h, char *c);
void dd(void);

int open(char *path, int flags);
int read (int fd, char *buf, int count);
int reads (int fd, char *buf, int count);
int readw(int fd, char *buf, int count); // read word from file (skrypt-style)
int lseek(int fd, int offset, int whence);
int PathToDir(char *path, char DirName[11]);
char *DirToPath(char filename[11], char *path);
unsigned long NextClu(unsigned long Clu);

char  *itoa (int w, char  *str, int radix);
int tolower (int ch);
int isgraph(int c);
int isprint(int c);
int isalnum (int c);
int isalpha (int c);
int iscntrl (int c);
int isdigit (int c);
int islower (int c);
int ispunct (int c);
int isspace (int c);
int isupper (int c);
int isxdigit(int c);

void readboot (char *buffer);
void out_boot(void *buf);
void process_boot(void *buf);
int mount_disk(unsigned char drive);
unsigned short int readsec0(char drive, char sec, char head, char trk /* or cyl */, char *Buffer);
unsigned short int writesec0(char drive, char sec, char head, char trk /* or cyl */, char *Buffer);
unsigned short int secread (int drive, unsigned AbsSec, char *Buffer);
unsigned short int secwrite (int drive, unsigned AbsSec, char *Buffer);
int SecForClu (int CluNo);
short int GetDriveParam (char drive);
short int GetDriveParam_bx (char drive);
short int GetDriveParam_cx (char drive);
short int GetDriveParam_dx (char drive);
short int get_boot_drive(void);

void tofile(void);
void tofile2(void);

void pause(void);

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

#if 0
struct __attribute__((__packed__)) Partition_stru
  {
  unsigned char   indicator;
        /*
        00  - non-bootable partition
        80  - bootable partition (one partition only)
        */
  unsigned char   begin_head;
  unsigned char   begin_sec; /* and 2 high bits of cylinder # */
  unsigned char   begin_cyl; /* low order bits of cylinder # */
        /*
        2 bytes are combined to a word similar to INT 13:

        ³7³6³5³4³3³2³1³0³ 1st byte  (sector)
         ³ ³ ÀÄÁÄÁÄÁÄÁÄÁÄÄ Sector offset within cylinder
         ÀÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄ High order bits of cylinder #

        ³7³6³5³4³3³2³1³0³ 2nd byte  (cylinder)
         ÀÄÁÄÁÄÁÄÁÄÁÄÁÄÄÄÄÄ Low order bits of cylinder #
         */
  unsigned char   system_indicator;
  unsigned char   end_head;
  unsigned char   end_sec; /* and 2 high bits of cylinder # */
  unsigned char   end_cyl; /* low order bits of cylinder # */
  unsigned int   preceding_sec;
  unsigned int   total_sec;
  };

struct __attribute__((__packed__)) MBRstru
  {
  char master_boot_loader [0x1BE];
  struct Partition_stru Partition [4];
  unsigned short int Signature; /* 55AA */
  };
#endif
