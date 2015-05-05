; INT 5 - Register dump

_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

sayr    macro   str
; В графических режимах не работает
        push    si
        lea     si,str  ; Было mov si,offset str
        call    sayr_proc
        pop     si
        endm    sayr

chr1    macro   sym     ; via BIOS
        ; Not worked in graph mode (bl - bg color!)
        ; NO SAVE REG
        mov     al,sym
        mov     ah,0eh
        int     10h
        endm    chr1

extrn   sayr_proc:near
extrn   ohw:near

_Int5dump       proc
        push    ax
        push    DS

        chr1    'c'
        chr1    's'
        chr1    '='

;        mov     ax,SS:[bp+4]
        push    CS
        pop     ax
        call    ohw

        chr1    ' '
        chr1    'd'
        chr1    's'
        chr1    '='
        mov     ax,DS
        call    ohw

        push    CS
        pop     DS

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
        mov     ax,[bp+4]
        call    ohw

        pop     DS
        pop     ax

        IRET
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
_Int5dump       endp
        public  _Int5dump

_TEXT   ends
        end
