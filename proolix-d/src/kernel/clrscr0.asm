        include macros.asm

_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_clrscr0        proc    near
        push    ax
        push    bx
        push    cx
        push    dx
        push    BP      ; Сохранение BP, который меняется одним из
                        ; вызовом int 10h

        sub     bh,bh
        mov     ah,08h
        int     10h

        mov     bl,bh
        mov     bh,ah
        sub     cx,cx
        mov     dx,184Fh
        mov     ax,0600h
        int     10h
        mov     bh,bl
        sub     dx,dx
        mov     ah,02h
        int     10h

        pop     BP
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        ret
_clrscr0        endp
        public _clrscr0

_TEXT   ends
        end