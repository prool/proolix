        include macros.asm

_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

extrn  sayr_proc:near
extrn  ohw:near
extrn  _saycsip:near

InterCept       proc
        mov     CS:@@BP,bp
        mov     bp,sp

        push    ax
        push    bx
        push    DS
        push    ES

        push    CS      ;
        pop     DS      ;

        sayr    @@s1

        push    bx
        push    ES
        push    ax

        mov     bx,SS:[BP]
        mov     ES,SS:[BP+2]
        sub     bx,2
        mov     ax,ES:[bx]

        call    ohw

        pop     ax
        pop     ES
        pop     bx

        call    _saycsip

        pop     ES
        pop     DS
        pop     bx
        pop     ax

        mov     BP,CS:@@BP
        iret

@@BP    dw      0
@@s1    db      13,10,'Int command= ',0
InterCept       endp
public  InterCept

_TEXT   ends
        end
