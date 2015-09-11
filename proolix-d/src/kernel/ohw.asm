; ohw

        locals

_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

        extrn   ohb:near

ohw     proc

;       Вывод слова в HEX-виде. Вход: слово в ax.
;       Все регистры сохраняются.
;       Вызывает подпрограмму ohb

        push    ax      ; Сохр. ради al.
        mov     al,ah
        call    ohb
        pop     ax      ; Восст. al.
        call    ohb
        ret
ohw     endp

        public  ohw

_TEXT   ends
        end
