_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_loop   proc
;        hlt
;        cli
@@l:    jmp     @@l
_loop   endp

public _loop

_TEXT   ends
        end
