_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_inport proc    near
        push    bp
        mov     bp,sp

        push    dx
        mov     dx,word ptr [bp+4]
        in      ax,dx
        pop     dx

;       mov     sp,bp
        pop     bp
        ret
_inport endp
public  _inport

_inportb        proc    near
        push    bp
        mov     bp,sp

        push    dx
        mov     dx,word ptr [bp+4]
        in      al,dx
        pop     dx

;       mov     sp,bp
        pop     bp
        ret
_inportb        endp
public  _inportb

_outport        proc    near
        push    bp
        mov     bp,sp

        push    dx
        mov     dx,word ptr [bp+4]
        mov     ax,word ptr [bp+6]
        out     dx,ax
        pop     dx

;       mov     sp,bp
        pop     bp
        ret
_outport        endp
public  _outport

_outportb       proc    near
        push    bp
        mov     bp,sp

        push    dx
        mov     dx,word ptr [bp+4]
        mov     al,byte ptr [bp+6]
        out     dx,al
        pop     dx

;       mov     sp,bp
        pop     bp
        ret
_outportb       endp
public  _outportb

_TEXT   ends
        end
