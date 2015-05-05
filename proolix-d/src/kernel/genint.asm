_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_geninterrupt   proc    near
        push    bp
        mov     bp,sp

        push    bx

        mov     bl,byte ptr [bp+4] ; interrupt no
        mov     byte ptr CS:@@cmd+1,bl
        jmp     @@cmd
@@cmd:  int     0

        pop     bx

;       mov     sp,bp
        pop     bp
        ret
_geninterrupt   endp
public  _geninterrupt

_TEXT   ends
        end
