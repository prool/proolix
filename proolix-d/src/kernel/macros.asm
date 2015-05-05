@GetChAtr       MACRO   page    ; Байт атрибутов символа -> AH
;               Предварительно должен быть известен номер текущей страницы.
;               Его надо поместить в параметр page.
;               По умолчанию номер страницы - 0.

;               См. макрос @GetAtr

                IFNB    <page>
                mov     bh,page
                ELSE
                sub     bh,bh
                ENDIF
                mov     ah,08h
                int     10h
                ENDM    @GetChAtr

@cls            MACRO
                ; Все регистры сохранены
                push    ax
                push    bx
                push    cx
                push    dx
                push    BP      ; Сохранение BP, который меняется одним из
                                ; вызовом int 10h

                @GetChAtr
                mov     bl,bh
                mov     bh,ah
                sub     cx,cx
                mov     dx,184Fh
                mov     ax,0600h
                int     10h
                mov     bh,bl
                sub     dx,dx
                mov     ah,02h
                int     10h

                pop     BP
                pop     dx
                pop     cx
                pop     bx
                pop     ax
                ENDM    @cls

                ; ОБЩЕУПОТРЕБИТЕЛЬНЫЕ МАКРОКОМАНДЫ

; (C) Copyright Serge Pustovoitoff, 1995
;       FidoNet 2:461/35
;       E-mail prool@infocom.kharkov.ua
;       BBS in Kharkov - phone 007 24h 5 lines
;       Почтовый адрес: 310168, Харьков, а/я 2996.

; Во всех макрокомандах данной части сохранены регистры (разумеется, кроме тех,
; в которые помещается выходной результат), ЕСЛИ В КОММЕНТАРИЯХ К САМОЙ
; МАКРОКОМАНДЕ НЕ СКАЗАНО ИНОЕ.

GetPage         MACRO   ; Номер тек. страницы -> BH
;               РЕГИСТРЫ НЕ СОХРАНЕНЫ: AX, BH
                mov     ah,0fh
                int     10h
                ENDM    GetPage

say     macro   str
; В графических режимах не работает
        push    si
        lea     si,str  ; Было mov si,offset str
        call    sayr_proc
        pop     si
        endm    say

sayr    macro   str
; В графических режимах не работает
        push    si
        lea     si,str  ; Было mov si,offset str
        call    sayr_proc
        pop     si
        endm    sayr

chr     macro   sym     ; via BIOS
        ; Not worked in graph mode (bl - bg color!)
        push    ax

        mov     al,sym
        mov     ah,0eh
        int     10h

        pop     ax
        endm    chr

chr1    macro   sym     ; via BIOS
        ; Not worked in graph mode (bl - bg color!)
        push    ax

        mov     al,sym
        mov     ah,0eh
        int     10h

        pop     ax
        endm    chr1

inkey   macro
        ; Ввод одного символа с клавиатуры с ожиданием без эха в регистр AL
        ; Если AL=0 то в AH засылается расширенный код иначе - в AH засылается
        ; скен-код.

        ; В отличие от inkey и inkey1 данный макрос требует ОДНОКРАТНОГО вызова
        ; для реакции на РАСШИРЕННЫЕ ASCII КЛАВИШИ.

        ; Во время ожидания реакция на CTRL-BREAK ЕСТЬ, реакции на CTRL-C нет.
        ; Во время ожидания popup программа TECH HELP вызывается.

        ; ПОРТИТ регистр AX

        mov     ah,00h
        int     16h

        endm    inkey

inkey2  macro
        ; Ввод одного символа с клавиатуры с ожиданием без эха в регистр AL
        ; Если AL=0 то в AH засылается расширенный код иначе - в AH засылается
        ; скен-код.

        ; В отличие от inkey и inkey1 данный макрос требует ОДНОКРАТНОГО вызова
        ; для реакции на РАСШИРЕННЫЕ ASCII КЛАВИШИ.

        ; Во время ожидания реакция на CTRL-BREAK ЕСТЬ, реакции на CTRL-C нет.
        ; Во время ожидания popup программа TECH HELP вызывается.

        ; ПОРТИТ регистр AX

        mov     ah,00h
        int     16h

        endm    inkey2
