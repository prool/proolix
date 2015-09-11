; Register dump

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

_saycsip        proc    near ; REG SAVED
        push    ax
        push    DS

        chr1    'i'
        chr1    'p'
        chr1    '='
        push    CS
        pop     ax
        call    ohw
        chr1    ':'
        call    ll
ll:     pop     ax
        call    ohw

        chr1    ' '
        chr1    'd'
        chr1    's'
        chr1    '='

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
        add     ax,6    ; исправление SP (с учетом call saycsip и двух push)
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
        public  _saycsip

_TEXT   ends
        end
