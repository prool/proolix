;       Proolix external asm-utility

;        include ..\kernel\macros.asm
        include syscall.h

;        .286

        .model tiny
        .code
;        org     0
;Revert  label   dword
;        org     100h

_main   proc    near    ; Это начало главной программы
        public  _main

;extrn   _saycsip:near
;        call    _saycsip

        ;       Точка А. (См. ниже)

;        pusha
;        popa

        mov     ax,'A'  ; Заталкиваем в стек первый (и единственный) параметр
        push    ax      ; системного вызова

        mov     ax,0deadh ; Сдвигаем стек на 1 слово для совместимости
        push    ax        ; с вызовами из программ на С

        mov     ax,putch        ; это системный вызов putch
        int     INT_NO

        add     sp,4    ; Возвращаем стек в исходное состояние, каким он был
                        ; в точке А
        ret             ; Возврат в Пруликс
_main   endp

        end