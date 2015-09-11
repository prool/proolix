
; struct sPSP     {  by Prool   comments from [Help PC]
; original from dos.h

CD20            dw      ? ;  machine code INT 20 instruction (CDh 20h)
TopMemPar       dw      ? ;  top of memory in segment (paragraph) form
Reserved        db      ? ;  reserved for DOS, usually 0
LongCall        db      5 dup (?) ;  machine code instruction long call to the DOS
                          ;  function dispatcher (obsolete CP/M)
Avail           dw      ? ;  .COM programs bytes available in segment (CP/M)
Int22           dd      ? ;  INT 22 terminate address; used by Proolix and MSDOS
Int23           dd      ? ;  INT 23 Ctrl-Break exit address; the original INT 23
                          ;  vector is NOT restored from this pointer (IP,CS)
Int24           dd      ? ;  INT 24 critical error exit address; the original
                          ;  INT 24 vector is NOT restored from this field (IP,CS)
ParentSeg       dw      ? ;  parent process segment addr (Undoc. DOS 2.x+)
                          ;  COMMAND.COM has a parent id of zero, or its own PSP
aHandle         db      20 dup (?) ;  file handle array (Undocumented DOS 2.x+); if handle
                          ;  array element is FF then handle is available.  Network
                          ;  redirectors often indicate remotes files by setting
                          ;  these to values between 80-FE.
aEnv            dw      ? ;  segment address of the environment, or zero (DOS 2.x+)
SS_SP           dd      ? ;  SS:SP on entry to last INT 21 function (Undoc. 2.x+)
sHandle         dw      ? ;  handle array size (Undocumented DOS 3.x+)
pHandle         dd      ? ;  handle array pointer (Undocumented DOS 3.x+)
prevPSP         dd      ? ;  pointer to previous PSP (deflt FFFF:FFFF, Undoc 3.x+)
DOS401          db 20 dup (?) ;  unused in DOS before 4.01
FnCB            db 3 dup (?) ;  DOS function dispatcher CDh 21h CBh (Undoc. 3.x+)
LList           dd      ? ; Proolix: far address of LList structure
                          ; DOS: unused
Unused          db 4 dup (?) ; DOS: unused
FCB1            db 16 dup (?) ;  default unopened FCB #1 (parts overlayed by FCB #2)
FCB2            db 19 dup (?) ;  default unopened FCB #2 (overlays part of FCB #1)
cLine_ctr       db ? ;  count of characters in command tail;  all bytes
                     ;  following command name;  also default DTA (128 bytes)
cLine           db 127 dup (?) ;     all characters entered after the program name followed
                     ;  by a CR byte
