/*      psp.h

        Program Segment Prefix (PSP) structure

        Copyright (c) Serge Pustovoitoff 1997

*/

struct sPSP     { /* by Prool */ /* comments from [Help PC] */
int     CD20; /* machine code INT 20 instruction (CDh 20h) */
unsigned int TopMemPar; /* top of memory in segment (paragraph) form */
char    Reserved; /* reserved for DOS, usually 0 */

/* 5 bytes - machine code instruction long call to the DOS */
/* function dispatcher (obsolete CP/M) */
char    op;
/* cpm_offset or .COM programs bytes available in segment */
unsigned int Avail;
unsigned int cpm_segment;

unsigned long Int22; /* INT 22 terminate address; used by Proolix and MSDOS */
unsigned long Int23; /* INT 23 Ctrl-Break exit address; the original INT 23 */
                     /* vector is NOT restored from this pointer (IP,CS) */
unsigned long Int24; /* INT 24 critical error exit address; the original */
                     /* INT 24 vector is NOT restored from this field (IP,CS) */
unsigned int ParentSeg; /* parent process segment addr (Undoc. DOS 2.x+) */
                        /* COMMAND.COM has a parent id of zero, or its own PSP */
char aHandle [20]; /* file handle array (Undocumented DOS 2.x+); if handle */
                        /* array element is FF then handle is available.  Network */
                        /* redirectors often indicate remotes files by setting */
                        /* these to values between 80-FE. */
unsigned int aEnv; /* segment address of the environment, or zero (DOS 2.x+) */
unsigned long SS_SP; /* SS:SP on entry to last INT 21 function (Undoc. 2.x+) */
unsigned int sHandle; /* handle array size (Undocumented DOS 3.x+) */
unsigned long pHandle; /* handle array pointer (Undocumented DOS 3.x+) */
unsigned long prevPSP; /* pointer to previous PSP (deflt FFFF:FFFF, Undoc 3.x+) */
char DOS401 [20]; /* unused in DOS before 4.01 */
char FnCB [3]; /* DOS function dispatcher CDh 21h CBh (Undoc. 3.x+) */
char Unused [9];
char FCB1 [16]; /* default unopened FCB #1 (parts overlayed by FCB #2) */
char FCB2 [20]; /* default unopened FCB #2 (overlays part of FCB #1) */
unsigned char cLine_ctr; /* count of characters in command tail;  all bytes */
                        /* following command name;  also default DTA (128 bytes) */
char cLine [127];    /* all characters entered after the program name followed */
                     /* by a CR byte */
};
