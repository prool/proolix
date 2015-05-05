_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

comment |
_wherex proc    near
        push    cx
        push    dx

        mov     ah,3
        int     10h
        xor     ax,ax
        mov     al,dl

        pop     dx
        pop     cx
        ret
_wherex endp

_wherey proc    near
        push    cx
        push    dx

        mov     ah,3
        int     10h
        xor     ax,ax
        mov     al,dh

        pop     dx
        pop     cx
        ret
_wherey endp
        |

_wherex proc    near
        push    ES

        mov     ax,40h
        mov     ES,ax
        xor     ax,ax
        mov     al,ES:[50h]

        pop     ES
        ret
_wherex endp

_wherey proc    near
        push    ES

        mov     ax,40h
        mov     ES,ax
        xor     ax,ax
        mov     al,ES:[51h]

        pop     ES
        ret
_wherey endp

public  _wherex
public  _wherey

_gotoxy proc    near
        push    bp
        mov     bp,sp

        push    bx
        push    dx

        call    VideoPage

        mov     dh,byte ptr [bp+6]
        mov     dl,byte ptr [bp+4]
        mov     ah,2
        int     10h

        pop     dx
        pop     bx

;       mov     sp,bp
        pop     bp
        ret
_gotoxy endp
public _gotoxy

VideoPage proc near
; ax - портит
; bh - video page

        mov     ah,0fh
        int     10h
        ; al ah - ...
        ; bh - video page

        ret
VideoPage endp
        public  VideoPage

_TEXT   ends
        end
