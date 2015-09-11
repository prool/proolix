; void jump_to_buffer(char *Buf);


_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_jump_to_buffer      proc    near
;        push    bp
        mov     bp,sp
        ; word ptr [bp+4] - argument 1
        ; word ptr [bp+6] - argument 2 ...

        jmp     word ptr [bp+4]

_jump_to_buffer      endp

public  _jump_to_buffer

_TEXT   ends
        end

