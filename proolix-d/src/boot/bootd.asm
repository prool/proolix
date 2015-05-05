        PAGE    ,79

; Debugging boot sector 0.0.0.4 25-Feb-96
; Copyright (C) Serge Pustovoitoff 1994, 1995

; History

; 0.0.0.4 25-Feb-96
; 0.0.0.3 16-Sep-95
; 0.0.0.3 15-Sep-95
; 0.0.0.2  8-Nov-94

off     equ     7c00h - 100h

; Макрос. Вывод одного символа через ROM BIOS.
chr1    macro   sym
        ; Not worked in graph mode (registr bl - background color!)
        ; NO SAVED REGS !!!
        mov     ax,0e00h+sym
        int     10h
        endm    chr1

sayr    macro   str
; NO SAVE REGS !!!

; Макрокоманда, аналогичная макрокоманде say. Только say использует функцию 9h
; ДОС, а sayr - функцию BIOS (функция 0eh прерывания 10h)
; В графических режимах не работает

        mov     si,offset str
        call    sayr_proc
        endm    sayr
        locals
        jumps

        .model  tiny
        .code
        org     100h
Begin:
                jmp     short l
                db      90h
;                       '12345678'
OEM             db      'KERNEL',0,0
SectSiz         dw      512
ClustSiz        db      2
ResSecs         dw      1
FatCnt          db      2
RootSiz         dw      112
TotSecs         dw      720
Media           db      0fdh
FatSize         dw      2
TrkSecs         dw      9
HeadCnt         dw      2
HidnSec         dd      0
BigNo           dd      0       ; Big total no of sectors
                dw      0       ; Physical drive no
                db      9       ; Extended boot signature
                ;dd     1       ; Volume serial no
FATaddr         dw      0       ; ! internal vars
StartClu        dw      0       ; !
Lbl             db      'No_Kernel',0,0   ; Volume label
                db      'FAT12 ' ; File system ID
RootSize        dw      '  '; use 2 last bytes of File system id
OldVec          dw      2 dup (0)

l: ; Table 11 bytes - drive param

        push    CS
        pop     DS

        call    saycsip
        sayr    s1+off
        mov     ax,FatSize+off
        call    ohw

        sayr    s2+off
        xor     ax,ax
        mov     ES,ax
        mov     ax,word ptr ES:[13h*4+2]
        call    ohw

l0:     jmp     l0

saycsip proc    ; REG SAVED
        push ax
        PUSHF

        sayr    s0+off
        push    CS
        pop     ax
        call    ohw
        chr1    ':'
        call    l1
l1:     pop     ax
        PUSH    BX
        mov     bx,offset l1-Begin
        sub     ax,bx
        call    ohw
        POP     BX

        sayr    s_ax+off
        pop     ax
        push    ax
        call    ohw

        sayr    s_bx+off
        mov     ax,bx
        call    ohw

        sayr    s_cx+off
        mov     ax,cx
        call    ohw

        sayr    s_dx+off
        mov     ax,dx
        call    ohw

        sayr    s_f+off
        POP     ax      ; flags (from stack) -> ax ; (see PUSHF)
        call    ohw

        sayr    s_si+off
        mov     ax,si
        call    ohw

        sayr    s_di+off
        mov     ax,di
        call    ohw

        sayr    s_ss+off
        mov     ax,SS
        call    ohw

        sayr    s_sp+off
        mov     ax,SP
        call    ohw

        sayr    s_ds+off
        mov     ax,DS
        call    ohw

        sayr    s_es+off
        mov     ax,ES
        call    ohw

        sayr    s_bp+off
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

        comment |
ints    proc    ; Reg no saved !!!
        sayr    s1+off

        mov     cx,0ffh
        mov     bp,0
l1:
        mov     al,0ffh
        sub     al,cl
        call    ohb

        chr1    ' '

        mov     ax,[bp+2]
        call    ohw

        chr1    ':'

        mov     ax,[bp]
        call    ohw

        chr1    13
        chr1    10

        add     bp,4
        test    cx,0fh
        jnz     l3
        inkey2
l3:
        loop    l1

        ret
s1      db      13,10,"ints",13,10,0
ints    endp
|
comment |
Int21   proc
        sayr    Ver+off
        iret
Int21   endp
|
comment |
dump    proc
        mov     cx,520
        xor     si,si
@@l:
        mov     ax,byte ptr Begin-base[bp+si]
        call    ohb
        inc     si
        loop    @@l
        ret
dump    endp
|
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

Buff    label   byte    ; for 512 byte space

        end     Begin
