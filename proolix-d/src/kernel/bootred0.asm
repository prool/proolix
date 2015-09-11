; read boot sector

_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

; bootread0 (int dev, void far * buf) return 0 or ErrorCode
_bootread0      proc    near
        push    bp
        mov     bp,sp
        ; word ptr [bp+4] - argument 1
        ; word ptr [bp+6] - argument 2 ...

        push    bx
        push    cx
        push    dx
        push    ES

        mov     dx,word ptr [bp+4]
        mov     bx,word ptr [bp+6]
        mov     ES,word ptr [bp+8]
        mov     cx,1
        mov     dh,0
        mov     ax,0201h        ; Fn=02, Read 1 sector
        int     13h

        jc      @@err
        xor     ax,ax
        jmp     @@ret
@@err:
@@ret:
        pop     ES
        pop     dx
        pop     cx
        pop     bx

;       mov     sp,bp
        pop     bp
        ret
_bootread0      endp

public  _bootread0

_TEXT   ends
        end
