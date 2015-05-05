_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_initv  proc

SET_INT MACRO   int_no, routine
        push    CS
        mov     ax,offset routine
        push    ax
        mov     ax,int_no
        push    ax
        call    _setvect
        add     sp,6
        ENDM

extrn  _setvect:near
extrn  InterCept:near

SET_INT 20h, InterCept ; general program termination
SET_INT 21h, InterCept ; function request services
SET_INT 22h, InterCept ; terminate address
SET_INT 23h, InterCept ; control break termination address
SET_INT 24h, InterCept ; critical error handler
SET_INT 25h, InterCept ; absolute disk read
SET_INT 26h, InterCept ; absolute disk write
SET_INT 27h, InterCept ; terminate and stay resident
SET_INT 28h, InterCept ; idle loop, issued by DOS when idle
SET_INT 29h, InterCept ; fast TTY console I/O
SET_INT 2Ah, InterCept ; critical section and NETBIOS
SET_INT 2Bh, InterCept ; internal, simple IRET in DOS 2.0-5.0
SET_INT 2Ch, InterCept ; "
SET_INT 2Dh, InterCept ; "
SET_INT 2Eh, InterCept ; exec command from base level command interpreter
SET_INT 2Fh, InterCept ; multiplexer

SET_INT 31h, InterCept ; DOS Protected Mode Interface (for DOS extenders)
SET_INT 32h, InterCept ; reserved
SET_INT 33h, InterCept ; mouse support
SET_INT 34h, InterCept ; Microsoft/Borland floating point emulation
SET_INT 35h, InterCept ; "
SET_INT 36h, InterCept ; "
SET_INT 37h, InterCept ; "
SET_INT 38h, InterCept ; "
SET_INT 39h, InterCept ; "
SET_INT 3Ah, InterCept ; "
SET_INT 3Bh, InterCept ; "
SET_INT 3Ch, InterCept ; "
SET_INT 3Dh, InterCept ; "
SET_INT 3Eh, InterCept ; "
SET_INT 3Fh, InterCept ; overlay manager

        ret
_initv  endp
public  _initv

_TEXT   ends
        end
