/*      dos.h

        Defines structs, unions, macros, and functions for dealing
        with MSDOS and the Intel iAPX86 microprocessor family.

        Copyright (c) Serge Pustovoitoff 1993-1997

*/

#if     !defined(__TIME_DATE_)
#define __TIME_DATE_

struct  time    {
        unsigned char   ti_min;         /* Minutes */
        unsigned char   ti_hour;        /* Hours */
        unsigned char   ti_hund;        /* Hundredths of seconds */
        unsigned char   ti_sec;         /* Seconds */
};

struct  date    {
        int             da_year;        /* Year - 1980 */
        char            da_day;         /* Day of the month */
        char            da_mon;         /* Month (1 = Jan) */
};
#endif

#if     !defined(__DOS_DEF_)
#define __DOS_DEF_

/* Variables */
extern  int              errno;
extern  char          ** environ;
extern  char           * sys_errlist[]; /* prool */

#define FA_RDONLY       0x01            /* Read only attribute */
#define FA_HIDDEN       0x02            /* Hidden file */
#define FA_SYSTEM       0x04            /* System file */
#define FA_LABEL        0x08            /* Volume label */
#define FA_DIREC        0x10            /* Directory */
#define FA_ARCH         0x20            /* Archive */
#define FA_ATTR2        0x40            /* Proolix Attr No.2 */
#define FA_ATTR1        0x80            /* Proolix Attr No.1 - Proolix file */

struct WORDREGS {
        unsigned int    ax, bx, cx, dx, si, di, cflag, flags;
};

struct BYTEREGS {
        unsigned char   al, ah, bl, bh, cl, ch, dl, dh;
};

union   REGS    {
        struct  WORDREGS x;
        struct  BYTEREGS h;
};

struct  SREGS   {
        unsigned int    es;
        unsigned int    cs;
        unsigned int    ss;
        unsigned int    ds;
};

struct  REGPACK {
        unsigned        r_ax, r_bx, r_cx, r_dx;
        unsigned        r_bp, r_si, r_di, r_ds, r_es, r_flags;
};

#define FP_OFF(fp)      ((unsigned)(fp))
#define FP_SEG(fp)      ((unsigned)((unsigned long)(fp) >> 16))

#define MK_FP(seg,ofs)  ((void far *) \
                           (((unsigned long)(seg) << 16) | (unsigned)(ofs)))

int       absread (int drive, int nsects, unsigned long lsect, void far *buffer); /* prool */
int       abswrite(int drive, int nsects, unsigned long lsect, void far *buffer); /* prool */
long      dostounix       (struct date *d, struct time *t);
void      getdate (struct date far *datep); /* prool */
void      gettime (struct time far *timep); /* prool */
void    interrupt       (*  getvect(int interruptno)) ();
/* void far * getvect (int VecNo); */
int       inport  (int portid);
unsigned char    inportb(int portid);
int       int86   (int intno, union REGS far *inregs,
                                    union REGS far *outregs); /* prool */
int       int86x  (int intno, union REGS *inregs, union REGS *outregs,
                         struct SREGS *segregs);
void      intr    (int intno, struct REGPACK *preg);
void      outport (int portid, int value);
void      outportb(int portid, unsigned char value);
int       peek    (unsigned segment, unsigned offset);
char      peekb   (unsigned segment, unsigned offset);
void      poke    (unsigned segment, unsigned offset, int value);
void      pokeb   (unsigned segment, unsigned offset, char value);
void      setdate (struct date *datep);
void      settime (struct time *timep);
void      setvect (int interruptno, void interrupt (*isr) ());

        /* These are in-line functions.  These prototypes just clean up
           some syntax checks and code generation.
         */

void      __cli__         (void);
void      __sti__         (void);
unsigned char  __inportb__(int portid);
void      __outportb__    (int portid, unsigned char value);
void      __int__         (int interruptnum);

#define disable()       __cli__()               /* Clear interrupt flag */
#define enable()        __sti__()               /* Set interrupt flag */
#define inportb(portid) __inportb__(portid)     /* Byte IN instruction */
#define outportb(portid, v) __outportb__(portid,v)/* Byte OUT instruction */
#define geninterrupt(i) __int__(i)              /* Interrupt instruction */

/* some other compilers use inp, outp for inportb, outportb */
#define inp(portid)     inportb(portid)
#define outp(portid,v)  outportb(portid,v)

#if !__STDC__

#define poke(a,b,c)     (*((int  far*)MK_FP((a),(b))) = (int)(c))
#define pokeb(a,b,c)    (*((char far*)MK_FP((a),(b))) = (char)(c))
#define peek(a,b)       (*((int  far*)MK_FP((a),(b))))
#define peekb(a,b)      (*((char far*)MK_FP((a),(b))))
#endif

#endif

#if     !defined(__FAT_CODES_)
#define __FAT_CODES_

#define FAT12           0
#define FAT16           1
#define NO_FAT          2
#define FAT32           3
#define FAT32LBA        4

#endif