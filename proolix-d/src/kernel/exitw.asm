; exit to Windows

_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

_exitw  proc    near    ; Это начало главной программы
        public  _exitw

;extrn   _saycsip:near
;        call    _saycsip

         mov     ax,4c00h
         int     21h

_exitw   endp

_TEXT   ends
        end
