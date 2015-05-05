_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

        extrn   VideoPage:near
        extrn   _TextAttr:byte

_putch1 proc    near
        push    bp
        mov     bp,sp

        push    ax
        push    bx
        push    cx
        push    dx

        call    VideoPage

        ; byte ptr [bp+4] - argument 1
        mov     al,byte ptr [bp+4]
        cmp     al,13
        je      @@cr
        cmp     al,10
        je      @@lf
        mov     ah,9
        mov     cx,1
        mov     bl,_TextAttr
        int     10h

        ; get cursor pos
        call    VideoPage
        mov     ah,3
        int     10h
        ; dh dl - cur pos (str, col)
        ; ch cl - cur size

        ; inc cursor pos
        inc     dl
        cmp     dl,80
        jae     @@nl    ; New line
        jmp     @@set_cur_pos
@@nl:
        mov     dl,0
        inc     dh
        cmp     dh,25
        jae     @@np    ; New page
        jmp     @@set_cur_pos
@@np:   mov     dh,0
        ; set cursor pos
@@set_cur_pos:  ; (dh,dl)
        mov     ah,2
        int     10h

        pop     dx
        pop     cx
        pop     bx
        pop     ax

;       mov     sp,bp
        pop     bp
        ret

@@cr:   ; get cursor pos
        call    VideoPage
        mov     ah,3
        int     10h
        ; dh dl - cur pos (str, col)
        ; ch cl - cur size
        mov     dl,0
        jmp     @@set_cur_pos

@@lf:   ; get cursor pos
        call    VideoPage
        mov     ah,3
        int     10h
        ; dh dl - cur pos (str, col)
        ; ch cl - cur size
        inc     dh
        cmp     dh,25
        jae     @@np1   ; New page
        jmp     @@set_cur_pos
@@np1:  mov     dh,0
        jmp     @@set_cur_pos

_putch1 endp
public  _putch1

_TEXT   ends
        end

