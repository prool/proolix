; read/write boot/master boot sector modules for bp.c

        locals

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

; bootwrite (int dev, void far * buf) return 0 or ErrorCode
_bootwrite      proc    near
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
        mov     ax,0301h        ; Fn=03, Write 1 sector
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
_bootwrite      endp

public  _bootwrite

_ReadPhysSec    proc
; int ReadPhysSec (unsigned char drive, unsigned char sec, unsigned char head,
; unsigned char trk /* or cyl */, char *Buffer);

;       /*
;       2 bytes are combined to a word similar to INT 13:
;
;       ³7³6³5³4³3³2³1³0³ 1st byte  (sector)
;        ³ ³ ÀÄÁÄÁÄÁÄÁÄÁÄÄ Sector offset within cylinder
;        ÀÄÁÄÄÄÄÄÄÄÄÄÄÄÄÄ High order bits of cylinder #
;
;       ³7³6³5³4³3³2³1³0³ 2nd byte  (cylinder)
;        ÀÄÁÄÁÄÁÄÁÄÁÄÁÄÄÄÄÄ Low order bits of cylinder #
;        */

; ALL REGS SAVED
        push    bp
        mov     bp,sp
        push    bx
        push    cx
        push    dx
        push    ES

        push    DS
        pop     ES

        mov     dl,byte ptr [bp+ 4] ; drive
        mov     cl,byte ptr [bp+ 6] ; sec
        mov     dh,byte ptr [bp+ 8] ; head
        mov     ch,byte ptr [bp+10] ; trk
        mov     bx,word ptr [bp+12] ; Buffer
        mov     ax,0201h; Fn=02, Read 1 sector
        int     13h
        jc      @@err
        xor     ax,ax
        jmp     @@ret

@@err:
;       mov     ax,-1
@@ret:

        pop     ES
        pop     dx
        pop     cx
        pop     bx
;       mov     sp,bp
        pop     bp

        ret
_ReadPhysSec    endp
public  _ReadPhysSec

_WritePhysSec    proc
; int WritePhysSec (unsigned char drive, unsigned char sec, unsigned char head,
; unsigned char trk, char *Buffer);

; ALL REGS SAVED
        push    bp
        mov     bp,sp
        push    bx
        push    cx
        push    dx
        push    ES

        push    DS
        pop     ES

        mov     dl,byte ptr [bp+ 4] ; drive
        mov     cl,byte ptr [bp+ 6] ; sec
        mov     dh,byte ptr [bp+ 8] ; head
        mov     ch,byte ptr [bp+10] ; trk
        mov     bx,word ptr [bp+12] ; Buffer
        mov     ax,0301h; Fn=03, Write 1 sector
        int     13h
        jc      @@err
        xor     ax,ax
        jmp     @@ret

@@err:
;       mov     ax,-1
@@ret:

        pop     ES
        pop     dx
        pop     cx
        pop     bx
;       mov     sp,bp
        pop     bp

        ret
_WritePhysSec    endp
public  _WritePhysSec

_TEXT   ends
        end
