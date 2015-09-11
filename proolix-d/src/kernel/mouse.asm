Titlo   equ     ' Mouse Package for XProolix'

;

_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

        db      ' Mouse port='
_PORT   dw      1       ; 1 == COM2
public  _PORT

_TEXT   ends
        end
