_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT
extrn   _RestoreVec:near

_run_boot  proc near
; void run_boot(int dev,int head,int sec,int trk);
;                   1       2        3       4
        push    bp
        mov     bp,sp
        ; word ptr [bp+ 4] - argument 1
        ; word ptr [bp+ 6] - argument 2 ...
        ; word ptr [bp+ 8] - argument 3 ...
        ; word ptr [bp+10] - argument 4 ...

;       вернуть старые прерывания
        cli
        call    _RestoreVec
        sti

        xor     ax,ax
        mov     ES,ax

        ; Read MBR
        mov     dl,byte ptr [bp+ 4]  ; drive
        mov     dh,byte ptr [bp+ 6]  ; head

        mov     cl,byte ptr [bp+ 8]  ; sec
        mov     ch,byte ptr [bp+10]  ; trk

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

_run_boot  endp
public _run_boot

_TEXT   ends
        end
