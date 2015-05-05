_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_end    proc    near

;;        extrn   Finish:near

;;        mov     ax,offset Finish
        push    CS
        pop     dx

        ret
_end    endp
public  _end

_TEXT   ends
        end
