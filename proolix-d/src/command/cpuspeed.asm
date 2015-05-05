; -----------------------------------------------------------------------------
; CPUSPEED.ASM  CPU speed measurement routine           Version 1.14
;
; Too-Much-In-One-So-Don't-Get-Lost(tm) CPU/FPU feature detection library
;
; Copyright(c) 1993,94 by B-coolWare.
; -----------------------------------------------------------------------------
; History:
;
;       11/11/93        slightly modified to support C/C++, slightly
;                       optimized for 286/386 instruction sets,
;                       documented where possible.
;       15/03/94        slightly modified to disable invalid computation for
;                       unknown CPUs
;       08/06/94        shortened by replacing repeated commands by DUPed
;                       equate
;       13/06/94        new CPU type addition reflected

;       MODEL   TPASCAL         ; uncomment this to use with Turbo Pascal
        MODEL   TINY,C          ; uncomment this to use with C/C++ compilers
;               ^^^^^ change to preferred memory model

INCLUDE UNIDEF.INC              ; macros

        LOCALS
        .data

        EXTRN   CPUFix: DWORD           ; CPU Type speed fix constant
        EXTRN   Shift : WORD            ; current loop length

        .code

ifndef  __use_386__
 ifdef  __use_286__
        .286
 endif
else
 __use_286__    equ     1
        .386
endif

        PUBLIC  Speed

_bpcs   equ     <byte ptr cs:>
_wp     equ     <word ptr>

clr     macro   reg
        sub     reg,reg
        endm


; -----------------------------------------------------------------------
; Speed modifies CPUFix and Shift and returns value which should b used
; as follows:
;
; CPUSpeed_In_MHz := (((CPUFix*Shift)/Speed)+5)/10;
;
; This formulae is taken from Norton SysInfo unchanged. I've no idea why
; computations done this way, but it works ok.
; I dinna insert floating point here, but to use with assembler you'll have to
; do so.
; Shift should be converted to DWORD prior to multiplication.
; Resulting value may be rounded to nearest integer, or may be left unchanged
; if you want exact CLK frequency value.

; function Speed( CPUid : Byte ) : Word;
; unsigned int Speed( unsigned short CPUid );

; CPUid is value from 0 to 0Eh identifying processor we're running on
; 8088   = 00h
; 8086   = 01h
; V20    = 02h
; V30    = 03h
; 188    = 04h
; 186    = 05h
; 286    = 06h
; 386sx  = 07h
; 386dx  = 08h
; 386sl  = 09h
; 486sx  = 0Ah
; 486dx  = 0Bh
; 486slc = 0Ch
; 486dlc = 0Dh
; P5     = 0Eh
; CxM1   = 0Fh
; It is lower byte of the result of CPU_Type routine defined in CPU_TYPE.ASH

MaxCPUid        equ     0Fh     ; change this if you added new CPU types

Speed  PROC
ARG     CPUid:BYTE
        cmp     CPUid,MaxCPUid  ; checking if we know this CPU
        jbe     @@okcpu
        mov     CPUid,MaxCPUid  ; fixing CPU id
@@okcpu:
        mov     cx,2
        mov     _bpcs[Indic],0
@@1:
        mov     Shift,cx
        call    Speed_Test
        cmp     ax,1000h
        jnb     @@2
        mov     cx,Shift
        shl     cx,1
        jmp     @@1
@@2:
        push    ax
        mov     cx,Shift
        mov     _bpcs[Indic],1
        call    Speed_Test
        pop     dx
        sub     dx,ax
        xchg    ax,dx
ifdef   __use_386__
        movzx   bx,CPUid
else
        mov     bl,CPUid
        sub     bh,bh
endif
ifdef   __use_286__
        shl     bx,2
else
        shl     bx,1
        shl     bx,1
endif
ifdef   __use_386__
        mov     edx,cs:CPUFixes[bx]
        mov     CPUFix,edx
else
        mov     dx,_wp cs:CPUFixes[bx]
        mov     _wp CPUFix,dx
        mov     dx,_wp cs:CPUFixes[2][bx]
        mov     _wp CPUFix[2],dx
endif
        ret
        ENDP

Speed_Test      PROC    NEAR
; returns number of tick-tacks spent performing known instruction?
; or difference between time taken by plain group of similar instructions and
; time taken by loop of that instructions? I dinna find out what exactly it
; does, but it works - what else do i (and u) need?

        PUSH    SI DI
        clr     DX,DX   ; CLR DX,DX
        MOV     SI,0AAAAH
        MOV     BX,05555H
        IN      AL,61h
        JMP     $+2
        AND     AL,0FCH
        OUT     61h,AL
        JMP     $+2
        MOV     AL,0B4h
        OUT     43h,AL
        JMP     $+2
        SUB     AL,AL
        OUT     42h,AL
        JMP     $+2
        OUT     42h,AL
        JMP     $+2
        IN      AL,61h
        MOV     DI,AX
        OR      AL,01
        CMP     _bpcs[Indic],0
        JZ      @@1
        JMP     @@2
@@1:
        CLI
        OUT     61h,AL
@@3:
Sprite  equ     <8Bh, 0C6h, 0F7h, 0F3h>
;       MOV     AX,SI
;       DIV     BX
        db      101     dup(Sprite)
        DEC     CX
        JZ      @@4
        JMP     @@3
@@2:
        CLI
        OUT     61h,AL
        NOP                     ; well, maybe this is the right place for
                                ; NOP, so i left it here. Try to kill it and
                                ; see what will happen...
@@5:
        db      Sprite
        DEC     CX
        JZ      @@4
        JMP     @@5

@@4:
        MOV     AX,DI
        OUT     61h,AL
        JMP     $+2
        STI
        IN      AL,42h
        JMP     $+2
        XCHG    AH,AL
        IN      AL,42h
        JMP     $+2
        XCHG    AH,AL
        NEG     AX
        PUSH    AX
        IN      AL,61h
        JMP     $+2
        AND     AL,0FDh
        OUT     61h,AL
        POP     AX DI SI
        RET
        ENDP
Indic   db      ?

CPUFixes:

; For on different processors the same instruction takes different number of
; CPU clocks, we should adjust absolute timer value using our knowledge of
; known CPU's timings.

        dd      2AD26h          ; 8088
        dd      2AD26h          ; 8086
        dd      0E90Bh          ; V20
        dd      0DFB9h          ; V30
        dd      0BA6Fh          ; 188
        dd      0BA6Fh          ; 186
        dd      06FDCh          ; 286
        dd      07486h          ; 386sx
        dd      07486h          ; 386dx
        dd      07486h          ; 386sl         ??? - ain't checked yet
        dd      07486h          ; 486sx
        dd      07486h          ; 486dx
        dd      0668Ah          ; Cx486slc
        dd      0668Ah          ; Cx486dlc
        dd      0668Ah          ; Pentium  ??? this value should be adjusted
        dd      0668Ah          ; CxM1     ??? this value should be adjusted

include cpu_type.ash

        END
