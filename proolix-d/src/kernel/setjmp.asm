        .8086

_TEXT     segment byte public 'CODE'
        assume  cs:_TEXT

;        include macros.asm

; int     setjmp(jmp_buf jmpb)

_setjmp proc    far     ; SP = old SP - 6 (2 bytes - jmpb addr, 4 bytes -
                        ; retf addr)
        push    bp      ; SP = old SP - 8
        mov     bp,sp   ; BP = old SP - 8      ; old BP = [BP]
                        ; old IP = [BP+2]
                        ; old CS = [BP+4]
                        ; jmpb addr = [BP+6]

        pushf           ; SP = old SP - 12      ; flag = [BP- 2]
        push    ax      ; SP = old SP - 14      ; ax   = [BP- 4]
        push    bx      ; SP = old SP - 16      ; bx   = [BP- 6]
        push    cx      ; SP = old SP - 18      ; cx   = [BP- 8]
        push    dx      ; SP = old SP - 20      ; dx   = [BP-10]
        push    si      ; SP = old SP - 22      ; si   = [BP-12]
        push    di      ; SP = old SP - 24      ; di   = [BP-14]
        push    ds      ; SP = old SP - 26      ; ds   = [BP-16]
        push    es      ; SP = old SP - 28      ; es   = [BP-18]

;        mov     ax,SETJMP_DATA
;        mov     ds,ax

        mov     bx,word ptr [bp+6]      ; bx - adress of jmpb

        mov     ax,BP
        add     ax,8
        mov     word ptr [bx+ 0],ax         ;j_sp
        mov     word ptr [bx+ 2],ss         ;j_ss
        mov     ax,[BP-2]
        mov     word ptr [bx+ 4],ax         ;j_flag
        mov     ax,[BP+4]
        mov     word ptr [bx+ 6],ax         ;j_cs
        mov     ax,[BP+2]
        mov     word ptr [bx+ 8],ax         ;j_ip
        mov     ax,[BP]
        mov     word ptr [bx+10],ax         ;j_bp
        mov     word ptr [bx+12],di         ;j_di
        mov     ax,[BP-18]
        mov     word ptr [bx+14],ax         ;j_es
        mov     word ptr [bx+16],si         ;j_si
        mov     word ptr [bx+18],DS         ;j_ds
        mov     ax,[BP-4]
        mov     word ptr [bx+20],ax         ;j_ax
        mov     ax,[BP-6]
        mov     word ptr [bx+22],ax         ;j_bx
        mov     word ptr [bx+24],cx         ;j_cx
        mov     word ptr [bx+26],dx         ;j_dx

        pop     es
        pop     ds
        pop     di
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     ax
        popf

        xor     ax,ax

        pop     bp
        ret
_setjmp endp

; void    _Cdecl longjmp(jmp_buf jmpb, int retval);

_longjmp proc far
        mov     bp,sp

;        chr1    'Q'

        mov     bx,word ptr [bp+4]      ; bx - adress of jmpb

        cli
        mov     ax,[bx]      ; j_sp
        mov     SP,ax
        mov     ax,[bx+2]    ; j_ss
        mov     SS,ax
        sti

        push     word ptr [bx+ 4]         ;j_flag
        push     word ptr [bx+ 6]         ;j_cs
        push     word ptr [bx+ 8]         ;j_ip
        push     word ptr [bx+10]         ;j_bp
        push     word ptr [bx+12]         ;j_di
        push     word ptr [bx+14]         ;j_es
        push     word ptr [bx+16]         ;j_si
        push     word ptr [bx+18]         ;j_ds
        push     word ptr [bx+20]         ;j_ax
        push     word ptr [bx+22]         ;j_bx
        push     word ptr [bx+24]         ;j_cx
        push     word ptr [bx+26]         ;j_dx

;        chr1    'q'

        pop     dx
        pop     cx
        pop     bx
        pop     ax
        pop     ds
        pop     si
        pop     es
        pop     di
        pop     bp
        IRET    ; == {pop ip; pop cs; popf}

_longjmp endp

_TEXT     ends

        public  _longjmp
        public  _setjmp

        end
