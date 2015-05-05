        include macros.asm
        locals

_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

extrn   ohw:near
extrn   _Trace:word
extrn   _Succ:word

debugl  proc    near
        pushf
        cmp     word ptr _Trace,0
        je      @@1
        cmp     ax,word ptr _Succ
        je      @@1
        mov     word ptr _Succ,ax
        call    ohw
        mov     ax,0e00h + ' '
        int     10h
@@1:
        popf
        ret
debugl  endp
        public debugl

debugb  proc    near
        push    ax
        pushf
        cmp     word ptr _Trace,0
        je      @@1

        mov     ax,0e00h + 'B'
        int     10h
@@1:
        popf
        pop     ax
        ret
debugb  endp
        public debugb

debuge  proc    near
        push    ax
        pushf
        cmp     word ptr _Trace,0
        je      @@1

        mov     ax,0e00h + 'E'
        int     10h
@@1:
        popf
        pop     ax
        ret
debuge  endp
        public debuge

_TEXT   ends
        end
