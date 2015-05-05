_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_getch0 proc    near
;        push    DS
        mov     ah,00h
        int     16h

;        xor     ah,ah
;        pop     DS
        ret
_getch0 endp
public  _getch0

_TEXT   ends
        end
