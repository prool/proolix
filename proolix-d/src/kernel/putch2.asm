; int putch3 (int ch)

        .8086
        jumps

_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

VIDEO_SEG       equ     0B800H

_curX   dw      1
_curY   dw      1
_maxX   dw      80
_maxY   dw      25
_curAddr        dd      0
_VideoAttrib    db      7
_LastRowOffset  dw      0

_putch2 proc    near
        push    bp
        mov     bp,sp

        ; SAVE ALL REGS
        push    ax
        push    bx
        push    cx
        push    dx
        push    si
        push    di
        PUSHF

        ; byte ptr [bp+4] - argument # 1

;int LastRowOffset, i;
        cld

;switch(ch)
;  {
;  case 0   : break;
        mov     ax,word ptr [bp+4]
        or      ax,ax
        jz      l_break
;  case '\r': break;
        cmp     ax,13
        je      l_break
;  case '\n': curX=1;
        cmp     ax,10   ; '\n'
        jne     l_default

        mov     _curX,1
;             curY++;
        inc     _curY
;             curAddr=MK_FP(VIDEO_SEG,maxX*(curY-1)*2);
        mov     word ptr _curAddr+2,VIDEO_SEG
        mov     ax,_curY
        dec     ax
        shl     ax,1
        mul     _maxX
        mov     word ptr _curAddr,ax
;             if (curY>maxY) goto scroll;
        mov     ax,_curY
        cmp     ax,_maxY
;        jg      l_scroll
;             break;
        jmp     l_break
;  default  : *curAddr++=ch;
l_default:
        les     di,_curAddr
        stosb
;             *curAddr++=VideoAttrib;
        mov     al,_VideoAttrib
        stosb
        add     word ptr _curAddr,2
;             if (++curX>maxX)
        mov     ax,_curX
        inc     ax
        mov     _curX,ax
        cmp     ax,_maxX
        jle     l_1
;               {
;               curX=1;
        mov     _curX,1
;               if (++curY>maxY)
        mov     ax,_curY
        inc     ax
        mov     _curY,ax
        cmp     ax,_maxY
        JMP     l_2     ;;;;;;;;;;;;;;;;;;;; jle     l_2
;                 {scroll:
l_scroll:
;                 curY=maxY;
        mov     ax,_maxY
        mov     _curY,ax
;                 curAddr-=maxX*2;
        mov     ax,word ptr _curAddr
        mov     bx,_maxX
        shl     bx,1
        sub     ax,bx
        mov     word ptr _curAddr,ax
;                 LastRowOffset=(maxY-1)*maxX*2;
        mov     ax,_maxY
        dec     ax
        mul     _maxX
        shl     ax,1
        mov     cx,ax
        mov     _LastRowOffset,ax
;                 memcpy(MK_FP(VIDEO_SEG,0),MK_FP(VIDEO_SEG,maxX*2),LastRowOffset);
        push    DS
        mov     ax,VIDEO_SEG
        mov     DS,ax
        mov     ES,ax
        mov     si,_maxX
        shl     si,1
        xor     di,di
        rep     movsb   ; DS:SI -> ES:DI
        pop     DS
;                 for (i=0;i<maxX*2;)
;                   {
;                   *(char far *)MK_FP(VIDEO_SEG,LastRowOffset+i++)=' ';
;                   *(char far *)MK_FP(VIDEO_SEG,LastRowOffset+i++)=7;
        mov     di,_LastRowOffset
        mov     cx,_maxX
        mov     ax,0720H
        rep     stosw
;                   }
;                 }
l_2:
;               }
l_1:
;  }
l_break:
;return ch;

        ; RESTORE REGS
        POPF
        pop     di
        pop     si
        pop     dx
        pop     cx
        pop     bx
        pop     ax

        pop     bp
        ret
_putch2 endp
public  _putch2

_TEXT   ends
        end


