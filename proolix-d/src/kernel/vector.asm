_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_getvect        proc    near
        push    bp
        mov     bp,sp

        push    DS
        push    bx
        push    cx

        xor     ax,ax
        mov     DS,ax

        mov     bx,word ptr [bp+4] ; interrupt no
        mov     cl,2
        shl     bx,cl
        cli
        mov     ax,[bx]
        mov     dx,[bx+2]
        sti

        pop     cx
        pop     bx
        pop     DS

;       mov     sp,bp
        pop     bp
        ret
_getvect        endp
public  _getvect

_setvect        proc    near
        push    bp
        mov     bp,sp

        push    ax
        push    bx
        push    cx
        push    DS

        xor     ax,ax
        mov     DS,ax

        mov     bx,word ptr [bp+4] ; interrupt no
        mov     cl,2
        shl     bx,cl

        cli
        mov     ax,word ptr [bp+6]
        mov     [bx],ax
        add     bx,2
        mov     ax,word ptr [bp+8]
        mov     [bx],ax
        sti

        pop     DS
        pop     cx
        pop     bx
        pop     ax

;       mov     sp,bp
        pop     bp
        ret
_setvect        endp
public  _setvect

_TEXT   ends
        end
