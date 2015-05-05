Titlo   equ     ' NoCMOS Kernel Package '
Ver     equ     ' V 0.0.0.2  2-Nov-1994 '

; CMOS & Real Time Clock asm functions for Proolix's Kernel

_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_ReadTick       proc    near
        push    bp
        mov     bp,sp

        push    bx
        push    cx
        push    dx

; word ptr [bp+4] - argument 1
;              6             2
; ...

; int ReadTick (unsigned long *Tick, char *Flag)

        mov     ah,0
        int     1ah
        jc      @@ret   ; error

        mov     bx,word ptr [bp+4]
        mov     word ptr [bx],dx
        mov     word ptr [bx+2],cx

        mov     bx,word ptr [bp+6]
        mov     byte ptr [bx],al

        xor     ax,ax
        jmp     @@ret

@@err:  mov     ax,1

@@ret:
        pop     dx
        pop     cx
        pop     bx

;       mov     sp,bp
        pop     bp
        ret
_ReadTick       endp
public  _ReadTick

_TEXT   ends
        end
