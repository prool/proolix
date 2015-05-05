Titlo   equ     ' CMOS Kernel Package '
Ver     equ     ' V 0.0.0.1 12-Oct-1994 '

; CMOS & Real Time Clock asm functions for Proolix's Kernel

        locals

_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_readRTC        proc    near
        push    bp
        mov     bp,sp

        push    bx
        push    cx
        push    dx

; word ptr [bp+4] - argument 1
;              6             2
; ...

; int readRTC (char *Hour, char *Min, char *Sec, char *Flag)

        mov     ah,2
        int     1ah
        jc      @@ret   ; error

        mov     bx,word ptr [bp+4]
        mov     byte ptr [bx],ch

        mov     bx,word ptr [bp+6]
        mov     byte ptr [bx],cl

        mov     bx,word ptr [bp+8]
        mov     byte ptr [bx],dh

        mov     bx,word ptr [bp+0ah]
        mov     byte ptr [bx],dl

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
_readRTC        endp
public  _readRTC

_readRTCdate    proc    near
        push    bp
        mov     bp,sp

        push    bx
        push    cx
        push    dx

; word ptr [bp+4] - argument 1
;              6             2
; ...

; int readRTCdate (char *Century, char *Year, char *Month, char *Day)

        mov     ah,4
        int     1ah
        jc      @@ret   ; error

        mov     bx,word ptr [bp+4]
        mov     byte ptr [bx],ch

        mov     bx,word ptr [bp+6]
        mov     byte ptr [bx],cl

        mov     bx,word ptr [bp+8]
        mov     byte ptr [bx],dh

        mov     bx,word ptr [bp+0ah]
        mov     byte ptr [bx],dl

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
_readRTCdate    endp
public  _readRTCdate

_TEXT   ends
        end
