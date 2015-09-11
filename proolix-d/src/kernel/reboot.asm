_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_reboot proc    near
        mov     ax,40h
        mov     ds,ax
        mov     bx,72h
        mov     [bx],1234h

        db      0eah            ; JMP   F000:FFF0
        dw      0fff0h,0f000h

_reboot endp

public  _reboot

_TEXT   ends
        end
