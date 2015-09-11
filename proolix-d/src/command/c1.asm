; Main module (CRT) for Proolix external C-utilities 0xDEAD (.com)
; V 0.0.2.0 14-Dec-1997

; History in stack order
; V 0.0.2.0 14-Dec-1997 - изменен формат исполнимого файла
; V 0.0.1.0  3-Mar-1997
; V 0.0.0.14 2-Feb-1997

sayr    macro   str
; NO SAVE REGS !!!

; Макрокоманда, аналогичная макрокоманде say. Только say использует функцию 9h
; ДОС, а sayr - функцию BIOS (функция 0eh прерывания 10h)
; В графических режимах не работает

        mov     si,offset str
        call    sayr_proc
        endm    sayr

chr     macro   sym     ; via BIOS
        ; Not worked in graph mode (bl - bg color!)
        push    ax

        mov     al,sym
        mov     ah,0eh
        int     10h

        pop     ax
        endm    chr

                extrn   _main:near

_TEXT   segment byte public 'CODE'
DGROUP  group   _TEXT,_DATA,_BSS

                org     0

include ..\include\psp.inc

Revert          =       Int22
_environ        =       aEnv
_More           =       DOS401  ; dw
_NLine          =       DOS401+2; dw

        public  Revert
        public  _environ
        public  _More
        public  _NLine

                org     100h
                assume  cs:DGROUP,ds:DGROUP
start:
        db      0deh, 0adh ; signature (magick)
;       mov     bp,sp
        push    CS
        pop     ax
        mov     DGROUP@,ax

;        call    saycsip

        call    _main   ; Вызов main-функции С

; Возврат в Пруликс
        jmp     dword ptr Revert

DGROUP@ dw      ?
public  DGROUP@

comment |
saycsip proc    ; REG SAVED
        push ax
        PUSHF

        sayr    s0
        push    CS
        pop     ax
        call    ohw
        chr    ':'
        call    l1
l1:     pop     ax
        PUSH    BX
;        mov     bx,offset l1-Begin
;        sub     ax,bx
        call    ohw
        POP     BX

        sayr    s_ax
        pop     ax
        push    ax
        call    ohw

        sayr    s_bx
        mov     ax,bx
        call    ohw

        sayr    s_cx
        mov     ax,cx
        call    ohw

        sayr    s_dx
        mov     ax,dx
        call    ohw

        sayr    s_f
        POP     ax      ; flags (from stack) -> ax ; (see PUSHF)
        call    ohw

        sayr    s_si
        mov     ax,si
        call    ohw

        sayr    s_di
        mov     ax,di
        call    ohw

        sayr    s_ss
        mov     ax,SS
        call    ohw

        sayr    s_sp
        mov     ax,SP
        call    ohw

        sayr    s_ds
        mov     ax,DS
        call    ohw

        sayr    s_es
        mov     ax,ES
        call    ohw

        sayr    s_bp
        mov     ax,BP
        call    ohw

        pop     ax
        ret
s0      db      13,10,"Bg CS:IP=",0
s1      db      " FatSize=",0
s2      db      " int 13h =",0
s_ax    db      ' ax=',0
s_bx    db      ' bx=',0
s_cx    db      ' cx=',0
s_dx    db      ' dx=',0
s_si    db      ' si=',0
s_di    db      ' di=',0
s_ss    db      ' SS=',0
s_sp    db      ' SP=',0
s_ds    db      ' DS=',0
s_es    db      ' ES=',0
s_bp    db      ' BP=',0
s_f     db      ' Flags=',0
saycsip endp

sayr_proc       proc    ; Ver 0.0.1 9-Dec-93
                        ; Процедура вывода строки при помощи функций BIOS
                        ; Вход: DS:SI - ASCIIZ строка.
                        ; NO REG SAVED !!!
; В графических режимах не работает
;       cld
sayr_l1:
        lodsb
        or      al,al
        jz      sayr_ret
        mov     ah,0eh
        int     10h
        jmp     short sayr_l1
sayr_ret:
        ret
sayr_proc       endp

ohw     proc

;       Вывод слова в HEX-виде. Вход: слово в ax.
;       Все регистры сохраняются.
;       Вызывает подпрограмму ohb

        push    ax      ; Сохр. ради al.
        mov     al,ah
        call    ohb
        pop     ax              ; Восст. al.
        call    ohb
        ret
ohw     endp
ohb     proc

; Procedure output hex byte Ver 0.1.1 6 Dec 93 via BIOS
; Input: AL - byte
; All regs. reserved ;)

; Not worked in graph mode. bl - bg color !!!

        push    ax
        push    cx
        push    dx

        mov     dl,al
        mov     cl,4
        shr     al,cl
        call    ohb1

        mov     al,dl
        and     al,0fh
        call    ohb1

        pop     dx
        pop     cx
        pop     ax

        ret

ohb     endp

ohb1    proc    ; Regs not saved !!!
        push    ax

        cmp     al,9
        ja      @@_1    ; al > 9
        ; al <= 9
        add     al,'0'
        jmp     @@_out

@@_1:   add     al,'A'-10
@@_out: mov     ah,0eh
        int     10h

        pop     ax

        ret
ohb1    endp


        |

_TEXT   ends

_DATA   segment word public 'DATA'
_DATA   ends

_BSS    segment word public 'BSS'
_BSS    ends

        end start
