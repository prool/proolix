;

        locals

_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_kbhit0 proc    near

        mov     ah,1
        int     16h
        jz      @@no
        mov     ax,1
        jmp     @@ret
@@no:
        xor     ax,ax
@@ret:
        ret
_kbhit0 endp
        public  _kbhit0

_TEXT   ends
        end
