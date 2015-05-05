_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_hdd2  proc
extrn   _RestoreVec:near

;       вернуть старые прерывания
        cli
        call    _RestoreVec
        sti

        xor     ax,ax
        mov     ES,ax

        ; Read MBR
        mov     dl,81h  ; drive
        mov     dh,0    ; head

        mov     cl,1    ; sec
        mov     ch,0    ; trk

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

_hdd2  endp
public _hdd2

_TEXT   ends
        end
