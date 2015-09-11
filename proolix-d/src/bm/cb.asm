; Startup module for Proolix /boot  (similar c0t.obj from Turbo C 2.0 and
;                                    similar crt.o from UNIXs)
; Version 0.0.1.7  23-Apr-97

; History

; 0.0.1.7  23-Apr-97
; 0.0.1.6  22-Apr-97
; 0.0.1.5  20-Apr-97 отловлен большой глюк с mov SP, ...
; 0.0.1.4  10-May-96 add internal stack
; 0.0.1.3   9-Mar-96
; 0.0.1.2  27-Feb-96 ...
; 0.0.1.1  26-Feb-96 cb.arj
; 0.0.1.1  25-Feb-96 cb.arj
; 0.0.1.0  11-Jan-96 cb.arj
;
; 0.0.0.15  5-Nov-95 c0.asm
; 0.0.0.15  4-Nov-95 ...
; 0.0.0.14 15-Jan-95 c0.asm

sayr    macro   str
; В графических режимах не работает
        push    si
        lea     si,str  ; Было mov si,offset str
        call    sayr_proc
        pop     si
        endm    sayr

chr    macro   sym     ; via BIOS
        ; Not worked in graph mode (bl - bg color!)
        ; NO SAVE REG
        mov     al,sym
        mov     ah,0eh
        int     10h
        endm    chr

KernPSP       equ     50h

_TEXT   segment byte public 'CODE'
DGROUP  group   _TEXT,_DATA,_BSS
        org     100h
        assume  cs:DGROUP,ds:DGROUP     ; ,ss:DGROUP

        .8086

_Begin:
        mov     ax,CS
        mov     DS,ax
        mov     ES,ax
;        mov     DGROUP@,ax
        cli
        mov     SS,ax
        mov     SP,offset 0FFECH ; StackEnd ; 0fffeh
        sti

;        chr     'A'

;        call    _saycsip

        extrn   _main:near
        jmp     _main
_kernel_start:
        public  _kernel_start   ;
        mov     ax,KernPSP
        mov     DS,ax
        mov     ES,ax
        cli
        mov     SS,ax
        mov     SP,0fffeh
        sti
        db      0eah            ; /* KERNEL START: JMP 50:100 */
        dw      100h,KernPSP        ;

;DGROUP@ dw      0
;public  DGROUP@

; comment |
_saycsip        proc    near ; REG SAVED
        push    ax
        push    DS

        chr    'i'
        chr    'p'
        chr    '='
        push    CS
        pop     ax
        call    ohw
        chr    ':'
        call    ll
ll:     pop     ax
        call    ohw

        chr    ' '
        chr    'd'
        chr    's'
        chr    '='

        push    DS
        pop     ax
        call    ohw

        push    CS
        pop     DS

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

        sayr    s_es
        mov     ax,ES
        call    ohw

        sayr    s_bp
        mov     ax,BP
        call    ohw

        pop     DS
        pop     ax
        ret
;s0      db      13,10,"CS:IP=",0
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
_saycsip        endp

ohw     proc

;       Вывод слова в HEX-виде. Вход: слово в ax.
;       Все регистры сохраняются.
;       Вызывает подпрограмму ohb

        push    ax      ; Сохр. ради al.
        mov     al,ah
        call    ohb
        pop     ax      ; Восст. al.
        call    ohb
        ret
ohw     endp

sayr_proc       proc    ; Процедура вывода строки при помощи функций BIOS
                        ; Вход: DS:SI - ASCIIZ строка.                        ; All registers saved
                        ; В графических режимах не работает
                        ; ALL REGS SAVED!
        push    ax
        push    si
        PUSHF

        cld
sayr_l1:
        lodsb
        or      al,al
        jz      sayr_ret
        mov     ah,0eh
        int     10h
        jmp     short sayr_l1
sayr_ret:
        POPF
        pop     si
        pop     ax
        ret
sayr_proc       endp

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
;        |

_TEXT   ends

_DATA   segment byte public 'DATA'
_DATA   ends

_BSS    segment byte public 'BSS'
_BSS    ends

        end _Begin
