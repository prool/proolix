_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

        extrn   _TextAttr:byte
        extrn   VideoPage:near

_putch0 proc    near
        push    bp
        mov     bp,sp
        push    ax
        push    bx
        push    cx

        mov     al,byte ptr [bp+4]
        cmp     al,10
        je      l_CRLF
        cmp     al,13
        je      l_CRLF
        ; set color

        ; 09H писать символ/атрибут в текущей позиции курсора
        ; Вход:  BH = номер видео страницы
        ;   AL = записываемый символ
        ;   CX = счетчик (сколько экземпляров символа записать)
        ;   BL = видео атрибут (текст) или цвет (графика)
        ;        (графические режимы: +80H означает XOR с символом на экране)
        call    VideoPage
        mov     ah,9
        mov     al,' '
        mov     cx,1
        mov     bl,_TextAttr
        int     10h

        ; byte ptr [bp+4] - argument 1
        mov     al,byte ptr [bp+4]
l_CRLF: mov     ah,0eh
        int     10h

        pop     cx
        pop     bx
        pop     ax
        xor     ah,ah
        pop     bp
        ret
_putch0 endp
public  _putch0

_TEXT   ends
        end
