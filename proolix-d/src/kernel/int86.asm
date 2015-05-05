_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_int86  proc    near
        public  _int86
; int   int86   (int intno, union REGS far *inregs, union REGS far *outregs);

        push    bp
        mov     bp,sp
        ; save regs
        push    bx
        push    cx
        push    si
        push    di
        push    ES

;       unsigned int    ax, bx, cx, dx, si, di, cflag, flags;

        ; intno
        mov     al,byte ptr [bp+4]
        mov     byte ptr CS:Intno,al

        ; ax
        les     bx,dword ptr [bp+6]
        mov     ax,word ptr es:[bx]

        ; bx
        push    ax
        les     bx,dword ptr [bp+6]
        mov     ax,word ptr es:[bx+2]
        mov     bx,ax
        pop     ax

        ; cx
        push    bx
        les     bx,dword ptr [bp+6]
        mov     cx,word ptr es:[bx+4]
        pop     bx

        ; dx
        push    bx
        les     bx,dword ptr [bp+6]
        mov     dx,word ptr es:[bx+6]
        pop     bx

        ; si
        push    bx
        les     bx,dword ptr [bp+6]
        mov     si,word ptr es:[bx+8]
        pop     bx

        ; di
        push    bx
        les     bx,dword ptr [bp+6]
        mov     di,word ptr es:[bx+10]
        pop     bx

        ; cflag             es:[bx+12]

        ; flags
        push    bx
        les     bx,dword ptr [bp+6]
        push    word ptr es:[bx+14]
        popf
        pop     bx

        ; INT!

        db      0CDh
Intno:  db      0         ; int 0

        ; ax
        push    bx
        les     bx,dword ptr [bp+10]
        mov     word ptr es:[bx],ax
        pop     bx

        ; bx
        mov     ax,bx
        les     bx,dword ptr [bp+10]
        mov     word ptr es:[bx+2],ax

        ; cx
        les     bx,dword ptr [bp+10]
        mov     word ptr es:[bx+4],cx

        ; dx
        les     bx,dword ptr [bp+10]
        mov     word ptr es:[bx+6],dx

        ; si
        les     bx,dword ptr [bp+10]
        mov     word ptr es:[bx+8],si

        ; di
        les     bx,dword ptr [bp+10]
        mov     word ptr es:[bx+10],di

        ; flags
        pushf
        pop     ax
        les     bx,dword ptr [bp+10]
        mov     word ptr es:[bx+14],ax

        ; cflag
        and     ax,1
        les     bx,dword ptr [bp+10]
        mov     word ptr es:[bx+12],ax

        ; restore regs
        pop     ES
        pop     di
        pop     si
        pop     cx
        pop     bx

        mov     sp,bp
        pop     bp
        ret
        endp

        ends
        end
