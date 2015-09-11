/*      proto.h

        Proolix Kernel Internal Fuctions Prototypes

        by Serge Pustovoitoff 1996-2004

*/

void loop(void);
int mshercinit(void);

void saycsip (void);
void reboot (void);
int bootread0 (int Dev, void far * Buf);
int bootread  (int Dev, void far *Buf);
int secread  (int dev, unsigned long lsect /* 0 - first for C convention */,
  char *buf);
int secwrite (int dev, unsigned long lsect, char *buf);
int ReadPhysSec (unsigned char drive, unsigned char sec, unsigned char head,
                 unsigned char trk, void *Buffer);
int ReadPhysSec2 (unsigned char drive, unsigned char sec, unsigned char head,
                  unsigned char trk /* or cyl */,
                  void far *Buf);
int WritePhysSec (unsigned char drive, unsigned char sec, unsigned char head,
                 unsigned char trk, char *Buffer);
void far *end (void);
void msdos(void);
void initv(void);

unsigned char unpack(unsigned char c);

int PathToDir(const char far *path, struct dirent far *Dir);
char *DirToPath(struct dirent far *Dir, char *path);

void interrupt far sh  (void);
void interrupt far msh (void);

int FlushAll(void);
int LoadCache (unsigned long sec);
int NewCache (unsigned long sec);
int garbage (void);
int shutdown(void);
void reboot (void);
void msdos(void);
void ident(void);
int l (int argc, char far *argv[]);
void rm(int argc, char far *argv[] );
void mv(int argc, char far *argv[] );
void cp(int argc, char far *argv[] );
void e(int argc, char far*argv[]);
void mem(void);
void memd(void);
void cat (int argc, char far *argv[] );
void vec(int argc,char far *argv[]);
void de (void);
void mount(int NewDevice);
void umount(void);
unsigned com_address ( int portid ) ;

int readRTC (char *Hour, char *Min, char *Sec, char *Flag);
int readRTCdate (char *Century, char *Year, char *Month, char *Day);
int ReadTick (unsigned long *Tick, char *Flag);

void On  (char *str);
void Off (char *str);

unsigned long SecForClu (unsigned long CluNo);
unsigned long NextClu (unsigned long CluNo);
unsigned int NextClu2 (unsigned int CluNo);
int LinkClu (unsigned long CluNo, unsigned long NeXT);
unsigned long GetFreeClu(void);
unsigned long AppendClu(unsigned long CluNo);

int CluRead (int drive, unsigned long CluNo, void far * buffer);
int CluRead2 (int drive, unsigned int CluNo, void far * buffer);

int putch0 (int);
int putch1 (int);
int putch2 (int);
int putch3 (int);
int putchp (int);

int open2 (int h, const char far *path, int flag);

void sched (void);

int OutIntVector(int i);

#ifndef exe_header
#include <struct.h>
#endif

int ViewEXE (struct exe_header far *Buf);
void outMBR(char far *buf);

void interrupt done(void);
void interrupt CtrlAltDel(void);

void interrupt Int0(void);
void interrupt Int1(void);
void interrupt Int2(void);
void interrupt Int3(void);
void interrupt Int4(void);
void interrupt Int6(void);
void interrupt Int7(void);
void interrupt Int8(void);
void interrupt Int20(void);
void interrupt Int21(void);
void interrupt Int75(void);

void init (void);

void nulldev (void);
void FDDstrategy (void);
void HDDstrategy (void);

int execpix(void far *mem0,int len);
int execdos(void far *mem0,int len);

void SysCall1(void);

void Int5dump(void);
/* Hа самом деле это - interrupt far функции. Hо для успешной
сборки ядра в виде com-файла пришлось слукавить и вручную указать ядерный
сегмент при назначении адреса программы перехвата. (См. ниже в соотв.
вызовах setvect(INT_NO, ... ) */

void ps (void);

unsigned int stackend (void);
unsigned int kernel_begin (void);
unsigned int mem_top(void);

void interrupt SysCall2();
int TR (int curdev);
void print_mount (void);
void out_devices (void);
char dos_letter(int);
unsigned int par_size_of_block ( void far *);
void exitw (void);