_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_inicom_        proc    near
        push    bp
        mov     bp,sp

        PUSH    dx
        mov     dx,word ptr [bp+4]
        mov     al,byte ptr [bp+6]
        mov     ah,0
        int     14h
        POP     dx

;       mov     sp,bp
        pop     bp
        ret
_inicom_        endp
public _inicom_

_getcom_        proc    near
        push    bp
        mov     bp,sp

        ; word ptr [bp+4] - argument 1

        PUSH    dx
        mov     dx,word ptr [bp+4]
        mov     ah,2
        int     14h
        POP     dx

;       mov     sp,bp
        pop     bp
        ret
_getcom_        endp
public _getcom_

_putcom_        proc    near
        push    bp
        mov     bp,sp

        ; word ptr [bp+4] - argument 1
        ;              6             2

        PUSH    dx
        mov     dx,word ptr [bp+4]
        mov     al,byte ptr [bp+6]
        mov     ah,2
        int     14h
        POP     dx

;       mov     sp,bp
        pop     bp
        ret
_putcom_        endp
public _putcom_

_getcomstat_    proc    near
        push    bp
        mov     bp,sp

        ; word ptr [bp+4] - argument 1

        PUSH    dx
        mov     dx,word ptr [bp+4]
        mov     ah,3
        int     14h
        POP     dx

;       mov     sp,bp
        pop     bp
        ret
_getcomstat_    endp
public _getcomstat_

_TEXT   ends
        end
