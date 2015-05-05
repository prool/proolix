; Main module (CRT) for Proolix external C-utilities 0xDEAF (.exe)
; V 0.0.3.0 14-Feb-98

; Макрос. Вывод одного символа через ROM BIOS.
chr    macro   sym
        ; Not worked in graph mode (registr bl - background color!)
        ; NO SAVED REGS !!!
        mov     ax,0e00h+sym
        int     10h
        endm    chr

; History in stack order
; V 0.0.3.0 14-Feb-98
; V 0.0.2.0 14-Dec-97 - изменен формат исполнимого файла
; V 0.0.1.0  3-Mar-97
; V 0.0.0.14 2-Feb-97

                extrn   _main:near

_TEXT   segment byte public 'CODE'
DGROUP  group   _DATA,_BSS

                assume  cs:_TEXT,ds:DGROUP
start:
;        chr     'B'
;l:      jmp     l

        mov     ax,DGROUP
        mov     DS,ax
        mov     DGROUP@,ax
        call    _main   ; Вызов main-функции С

; Возврат в Пруликс
        xor     ax,ax
        push    ax
        extrn   _exit:near
        call    near ptr _exit

DGROUP@ dw      ?
public  DGROUP@

_TEXT   ends

_DATA   segment word public 'DATA'
_DATA   ends

_BSS    segment word public 'BSS'
_BSS    ends

_STACK    segment word public stack 'STACK'
        db      60 dup ('stack')
_STACK    ends

        end start
