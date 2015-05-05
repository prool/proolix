_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

;void load2go (unsigned char drive, unsigned char sec, unsigned char head,
;                 unsigned char trk);  /* load boot sector of partition and go this */

_load2go  proc
extrn   _RestoreVec:near
;        push    bp
        mov     bp,sp
        ; word ptr [bp+4] - argument 1
        ; word ptr [bp+6] - argument 2 ...


;       вернуть старые прерывания
        cli
        call    _RestoreVec
        sti

        xor     ax,ax
        mov     ES,ax

        ; Read MBR
        mov     dl,byte ptr [bp+4]    ; drive
        mov     dh,byte ptr [bp+8]    ; head

        mov     cl,byte ptr [bp+6]    ; sec
        mov     ch,byte ptr [bp+10]   ; trk

        mov     al,1    ; sec count
        mov     ah,2    ; Fn read

        mov     bx,7c00h
        int     13h
        nop
        jc      @@reboot

        ;jmp    0:7c00
        db      0eah
        dw      7c00h
        dw      0

@@reboot:; jmp  ffff:0
        db      0eah
        dw      0
        dw      0ffffh

_load2go  endp
public _load2go

_TEXT   ends
        end
