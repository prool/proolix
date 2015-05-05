; Startup module for Proolix kernel (similar to Turbo C 2.0's c0t.obj and
;                                    similar to UNIX's crt.o)

; History

; 0.0.2.15  2-Nov-99 add BeforeSS, BeforeSP
; 0.0.2.14 17-May-97
; 0.0.2.13 28-Apr-97 отловлен большой глюк в связи с тем, что ядро
;                    откомпилировано Turbo C 2.0 в модели памяти small,
;                    где должно быть DS=SS, а у меня было DS!=SS
; 0.0.2.12 28-Apr-97
; 0.0.2.11 21-Apr-97
; 0.0.2.10 20-Apr-97 отловлен большой глюк с mov SP, ...
; 0.0.2.9  22-Mar-97
; 0.0.2.8   8-Mar-97
; 0.0.2.7   3-Mar-97
; 0.0.2.6   2-Mar-96
; 0.0.2.5   2-Feb-96
; 0.0.2.4  14-Oct-96
; 0.0.2.3  29-Sep-96
; 0.0.2.2  16-May-96
; 0.0.2.1  11-May-96 начал делать control-break
; 0.0.2.0  21-Apr-96 _execexe
; 0.0.1.5   3-Apr-96
; 0.0.1.4  31-Mar-96
; 0.0.1.3  24-Mar-96
; 0.0.1.2  18-Mar-96
; 0.0.1.1  17-Mar-96
; 0.0.1.0  14-Mar-96 for EXE kernel

; 0.0.0.15  5-Nov-95 for com kernel
; 0.0.0.15  4-Nov-95
; 0.0.0.14 15-Jan-95

; Макрос. Вывод одного символа через ROM BIOS.
chr1    macro   sym
        ; Not worked in graph mode (registr bl - background color!)
        ; NO SAVE REGS !!!
        mov     ax,0e00h+sym
        int     10h
        endm    chr1

sayr    macro   str
; NO SAVE REGS !!!

; Макрокоманда, аналогичная макрокоманде say. Только say использует функцию 9h
; ДОС, а sayr - функцию ROM BIOS (функция 0eh прерывания 10h)
; В графических режимах не работает

        mov     si,offset str
        call    sayr_proc
        endm    sayr

extrn   _errno:word
extrn   _More:word
extrn   _NLine:word
extrn   _HeadCnt:word
extrn   _TrkSecs:word
extrn   _CurrentDevice:word

StackTopPar     equ     0fffh

_TEXT   segment byte public 'CODE'
DGROUP  group   _DATA,_BSS
        assume  cs:_TEXT,ds:DGROUP ; ,ss:DGROUP

        .8086

_Begin:

;                mov     DX,DGROUP
                mov     dx,_DATA

                mov     DGROUP@,dx
                mov     DS,dx

                cli
                mov     SS,dx
                mov     SP,StackTopPar shl 4
                sti

                mov     ax,ES:[4]
                mov     _CurrentDevice,ax

                extrn   _saycsip:near
                call    _saycsip

                extrn   _main:near
                jmp     _main

DGROUP@ dw      0
public  DGROUP@

KernelSS        dw      0
KernelSP        dw      0
_UserSS         dw      0
_UserDS         dw      0
UserSP          dw      0
SaveAX          dw      0
_Trace          dw      0
_Succ           dw      0

public          _UserSS
public          _UserDS
public          _Trace
public          _Succ

_stackend   proc    near
        mov     ax,DGROUP@
        add     ax,StackTopPar
        ret
_stackend   endp
        public  _stackend

_kernel_begin   proc    near
        mov     ax,seg _Begin
        ret
_kernel_begin   endp
        public  _kernel_begin

_execpix        proc    near

        push    bp
        mov     bp,sp

        push    ax
        push    bx
        push    cx
        push    dx
        push    si
        push    di
        push    DS
        push    ES

        mov     ax,word ptr [bp+6] ; segm

; exec(), выданный из режима ядра (_Mode=0, SS=70h) производит переход в
; режим пользователя (_Mode=1, SS=User's SS)

; exec(), выданный из режима пользователя, не производит изменения режима
; (это еще нужно продумать хорошо, а пока exec у меня происходит только из
; режима ядра с безусловным переходом в режим пользователя)

        ; SAVE SS SP
        CLI
        mov     CS:KernelSS,SS
        mov     CS:KernelSP,SP
        mov     CS:_Mode,1
        STI

        mov     DS,ax
        mov     ES,ax
        mov     word ptr CS:@@seg,ax
        mov     bx,0fffeh
        sub     bx,word ptr [bp+8] ; 0fffeh - len

        ; save Return address in PSP
        mov     cx,offset Return
        mov     ES:[12],cx
        mov     cx,CS
        mov     ES:[14],cx

        CLI
        mov     SS,ax
        mov     SP,bx
        STI

        ; EXEC!
        JMP     dword ptr StartAddr     ; было CALL
Return:
public  Return
        ; RESTORE SS SP
        CLI
        mov     SS,CS:KernelSS
        mov     SP,CS:KernelSP
        mov     CS:_Mode,0
        STI

        pop     ES
        pop     DS
        pop     di
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     ax

        pop     bp
        ret

StartAddr       dw      102h
@@seg   dw      0       ; seg
_execpix        endp

public  _execpix

_WritePhysSec    proc
; int WritePhysSec (unsigned char drive, unsigned char sec, unsigned char head,
; unsigned char trk, char *Buffer);

; ALL REGS SAVED
        push    bp
        mov     bp,sp
        push    bx
        push    cx
        push    dx
        push    ES

        push    DS
        pop     ES

        mov     dl,byte ptr [bp+ 4] ; drive
        mov     cl,byte ptr [bp+ 6] ; sec
        mov     dh,byte ptr [bp+ 8] ; head
        mov     ch,byte ptr [bp+10] ; trk
        mov     bx,word ptr [bp+12] ; Buffer
        mov     ax,0301h; Fn=03, Write 1 sector
        int     13h
        jc      @@err
        xor     ax,ax
        jmp     @@ret

@@err:
;       mov     ax,-1
@@ret:

        pop     ES
        pop     dx
        pop     cx
        pop     bx
;       mov     sp,bp
        pop     bp

        ret
_WritePhysSec    endp
public  _WritePhysSec

extrn   _User:word

_TEXT   ends

_DATA   segment word public 'DATA'
_Mode   dw      ?
public  _Mode
        ; System call table
_SysTable       label   word
public  _SysTable

include         syskern.asm

_DATA   ends

_BSS    segment word public 'BSS'
_NearBuffer     db      512 dup ('.')   ; for absread.c.
public          _NearBuffer             ; нужно не пересекать границу 64k
_BSS    ends

public  _Begin
        end _Begin