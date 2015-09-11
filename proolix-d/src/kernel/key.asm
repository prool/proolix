comment |

    Высылаю исходник редактора командной строки с историей для Пруликса.
    Поддерживаемые кнопки -
        влево,вправо,Home,End,Забой(Backspace),BK(Enter): как обычно;
        вверх,вниз : перемещение по истории;
        ESC : очистка строки и установка в начало буфера указателя истории;
        Ctrl-Left,Ctrl-Right,Ctrl-Backspace.

    Буфер истории кольцевой.Никаких АТ-шных приколов вроде нету,-должно 
работать на чем угодно, а кроме того буду проверять на наличие введенной 
строки в буфере истории и ставить ее первой,сдвигая, соответственно,весь 
буфер (редко встречал - толко в XTREE PRO).

    Проверить не удалось, не пишу я почти ничего на сях, а Borland C,вообще 
не люблю; стоит у меня Symantec C 6.0, написал маленький тестик - работает, а 
проблема может возникнуть в единственном месте - при копировании строки в 
выходную: ES должен быть выставлен правильно.  Транслировал TASM 3.2 c 
ключами "-Z -q -t -m -ml".

        FidoNet 2:461/61.99             Игорь Cазонов
        |

; Version 0.0.0.2 3-May-94

        .model tiny,pascal
        .8086
        jumps
        locals  @@

HISDEPTH        equ     10
MaxLen          equ     81
        .code
        db      0
strbuf  db      MaxLen dup (?)
hisbuf  db      MaxLen * HISDEPTH dup (0)
curr    dw      ?
cursor  dw      ?
depth   dw      0               ; текущая глубина буфера команд
current dw      offset hisbuf   ; текущая команда в буфере

HisHead         dw      offset hisbuf
HisCur          dw      offset hisbuf

global _doskey:proc

_doskey          PROC C
                USES di,si
                ARG  string
        push    ds es
        mov     ax,40h
        mov     es,ax
        mov     bx,63h
        mov     bx,es:[bx]
        cmp     bx,03b4h
        jnz     @@more
        mov     vseg,0b000h
@@more:
        mov     ax,cs
        mov     es,ax
        mov     ds,ax
        mov     ax,HisHead
        mov     HisCur,ax
        mov     curr,offset strbuf
        mov     strbuf,0
        xor     bh,bh
        mov     ah,3
        int     10h
        mov     cursor,dx
;
newkey: xor     ax,ax
        int     16h
        cmp     ax,0e7fh ; Ctrl+BackSpace
        jz      @@cbs

        cmp     al,' '
        jb      @@ctrl
;       символ
        push    ax
        cmp     curr,offset strbuf+80 ; на переполнение входной строки
        jae     newkey
        push    curr
        call    StrLen
        inc     cx
        mov     si,curr
        add     si,cx
        mov     di,si
        inc     di
        std
        inc     cx
        rep movsb
        pop     ax
        mov     di,curr
        mov     [di],al
        inc     curr
        call    FlushBuf
        jmp     newkey

@@ctrl: cmp     al,8    ; backspace
        jz      @@bs
        cmp     al,01bh ; ESC
        jz      @@esc
        cmp     ah,04bh ; left
        jz      @@left
        cmp     ah,04dh ; right
        jz      @@right
        cmp     ah,053h ; Del
        jz      @@del
        cmp     ah,047h ; Home
        jz      @@home
        cmp     ah,04fh ; End
        jz      @@end
        cmp     al,0dh  ; Enter
        jz      @@enter
        cmp     ah,048h ; Up
        jz      @@up
        cmp     ah,050h ; Down
        jz      @@down
        cmp     ah,073h ; Ctrl+left
        jz      @@cleft
        cmp     ah,074h ; Ctrl+right
        jz      @@cright

        jmp     newkey

@@bs:   cmp     curr,offset strbuf
        jz      newkey
        mov     cx,curr
        push    cx
        CALL    StrLen
        inc     cx
        mov     si,curr
        mov     di,si
        dec     di
        cld
        rep movsb
        dec     curr
        call    FlushBuf
        jmp     newkey

@@del:  mov     ax,curr
        mov     cx,offset strbuf
        sub     ax,cx
        push    cx
        CALL    StrLen
        cmp     ax,cx
        jae     newkey
        mov     cx,curr
        push    cx
        CALL    StrLen
        inc     cx
        mov     di,curr
        mov     si,di
        inc     si
        cld
        rep movsb
        call    FlushBuf
        jmp     newkey

@@esc:  mov     strbuf,0
        mov     curr,offset strbuf
        mov     ax,HisHead
        mov     HisCur,ax
        CALL    FlushBuf
        jmp     newkey

@@left: cmp     curr,offset strbuf
        jz      newkey
        dec     curr
        CALL    SetCur
        jmp     newkey

@@right:mov     ax,curr
        mov     cx,offset strbuf
        sub     ax,cx
        push    cx
        CALL    StrLen
        cmp     ax,cx
        jae     newkey
        inc     curr
        CALL    SetCur
        jmp     newkey

@@home: mov     curr,offset strbuf
        CALL    SetCur
        jmp     newkey

@@end:  mov     bx,offset strbuf
        push    bx
        CALL    StrLen
        add     bx,cx
        mov     curr,bx
        CALL    SetCur
        jmp     newkey

@@cleft:
        mov     bx,1
        std
        jmp     @@word
@@cright:
        xor     bx,bx
        cld
@@word:
        mov     si,curr
@@cl3:  lodsb
        or      al,bl
        jz      newkey
        cmp     byte ptr [si],0
        jz      @@cl1
        cmp     byte ptr [si],'1'
        jb      @@cl3
@@cl2:  lodsb
        cmp     al,'1'
        jb      @@cl4
        cmp     byte ptr [si],'1'
        jae     @@cl2
@@cl1:
        add     si,bx
@@cl4:  mov     curr,si
        CALL    SetCur
        jmp     newkey

@@up:   CALL    GetPrev
        jmp     @@his
@@down: CALL    GetNext
@@his:  cmp     byte ptr [bx],0
        jz      newkey
        mov     HisCur,bx
        push    bx
        push    offset strbuf
        CALL    StrCpy

        mov     bx,offset strbuf
        push    bx
        CALL    StrLen
        add     bx,cx
        mov     curr,bx
        CALL    SetCur
        CALL    FlushBuf
        jmp     newkey

@@cbs:
        cmp     curr,offset strbuf
        jz      newkey
        std
        mov     si,curr
@@bs3:  lodsb
        cmp     byte ptr [si],0
        jz      @@bs1
        cmp     byte ptr [si],'1'
        jb      @@bs3
@@bs2:  lodsb
        cmp     al,'1'
        jb      @@bs4
        cmp     byte ptr [si],'1'
        jae     @@bs2
@@bs1:
        inc     si
@@bs4:
        mov     di,si
        mov     si,curr
        mov     curr,di
        mov     cx,si
        sub     cx,di
        inc     cx
        cld
        rep movsb
        CALL    SetCur
        CALL    FlushBuf

        jmp     newkey


@@enter:CALL    AddHistory
        mov     ax,0e0dh
        xor     bh,bh
        int     10h
        mov     ax,0e0ah
        xor     bh,bh
        int     10h
        mov     ax,string
        push    offset strbuf
        push    ax
        CALL    StrCpy          ; !!! надо чтобы ES смотрел правильно
                                ; У меня Borland C нет, тестировалось
                                ; на Symantec C 6.0 - всё OK
        pop     es ds
        ret
_doskey          ENDP

FlushBuf        PROC
                USES ax,si
        xor     bh,bh
        mov     dx,cursor
        mov     ah,2
        int     10h

        mov     si,offset strbuf
        push    si
        CALL    StrLen

        push    es
        db      0B8h            ;mov    ax,VideoSeg
vseg    dw      0b800h
        mov     es,ax
        mov     ax,cursor
        mov     bx,ax
        mov     al,ah
        mov     ah,160
        mul     ah
        xor     bh,bh
        shl     bx,1
        add     bx,ax
        mov     di,bx

        or      cx,cx
        jz      @@empty

        cld
        push    cx
@@out:  lodsb
        stosb
        inc     di
        loop    @@out
        pop     cx

@@empty:neg     cx
        add     cx,79
        cld
@@blank:mov     al,020h
        stosb
        inc     di
        loop    @@blank
        pop     es

        CALL    SetCur
        ret
FlushBuf        ENDP

StrLen          PROC
                USES ax,di
                ARG  instring:word
        mov     di,instring
        xor     al,al
        cld
        mov     cx,0ffffh
        repne scasb
        neg     cx
        dec     cx
        dec     cx
        ret
StrLen          ENDP

StrCpy          PROC
                USES di,si,cx
                ARG  str1,str2
        mov     si,str1
        mov     di,str2
        push    si
        CALL    StrLen
        inc     cx
        cld
        rep movsb
        ret
StrCpy          ENDP

SetCur          PROC
                USES    ax,bx,dx
        mov     ax,curr
        sub     ax,offset strbuf
        mov     dx,cursor
        add     dl,al
        xor     bh,bh
        mov     ah,2
        int     10h
        ret
SetCur          ENDP

AddHistory      PROC
        push    offset strbuf
        push    HisHead
        CALL    StrCpy
        add     HisHead,MaxLen
        cmp     HisHead,offset curr
        jb      @@ret
        mov     HisHead,offset hisbuf
@@ret:  mov     ax,HisHead
        mov     HisCur,ax
        ret
AddHistory      ENDP

GetPrev         PROC    ; bx - addr
        mov     bx,HisCur
        cmp     bx,offset hisbuf
        jnz     @@nojmp
        mov     bx, offset hisbuf + (HISDEPTH - 1) * MaxLen
        jmp     @@ret
@@nojmp:sub     bx,MaxLen
@@ret:  ret
GetPrev         ENDP

GetNext         PROC    ; bx - addr
        mov     bx,HisCur
        cmp     bx, offset hisbuf + (HISDEPTH - 1) * MaxLen
        jnz     @@nojmp
        mov     bx,offset hisbuf
        jmp     @@ret
@@nojmp:add     bx,MaxLen
@@ret:  ret
GetNext         ENDP

        end
