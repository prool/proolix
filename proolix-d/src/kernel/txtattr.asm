_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

extrn   _TextAttr:byte

_textattr       proc    near
        push    bp
        mov     bp,sp

        ; word ptr [bp+4] - argument 1
        mov     al,byte ptr [bp+4]
        mov     _TextAttr,al

;       mov     sp,bp
        pop     bp
        ret
_textattr       endp
public _textattr

_getcolor       proc    near
        push    bp
        mov     bp,sp

        mov     al,_TextAttr
        xor     ah,ah

;       mov     sp,bp
        pop     bp
        ret
_getcolor       endp
public  _getcolor
_TEXT   ends
        end
