; ohw

ohw:

;       Вывод слова в HEX-виде. Вход: слово в ax.
;       Все регистры сохраняются.
;       Вызывает подпрограмму ohb

        push    ax      ; Сохр. ради al.
        mov     al,ah
        call    ohb
        pop     ax      ; Восст. al.
        call    ohb
        ret
