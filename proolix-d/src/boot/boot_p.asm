        PAGE    ,132
; Boot sector ver. 0.0.1.1  8-Nov-94 from Proolix operating system
; Copyright (C) Serge Pustovoitoff, 1993, 1994

off             equ     -100h
Drive           equ     0       ; Drive A
;ModifyParams    equ     'Yes'
Buff            equ     0       ; 500h  ?!!!!!!!!!!
NewSeg          equ     1050h
PSPSeg          equ     50h     ; 70h ???????????????????????????????
LoadAddr        equ     (PSPSeg shl 4)+100h
;#############################################################################
; Print character sym to tty
; APS revised
chr1    macro   sym             ; via BIOS
                                ; Not worked in graph mode (bl - bg color!)
                                ; NO SAVE REGS !!!
        mov     ax,0e00h+sym
        int     10h
        endm    chr1
;#############################################################################
; NO SAVE REGS !!!
; Макрокоманда, аналогичная макрокоманде say. Только say использует функцию 9h
; ДОС, а sayr - функцию BIOS (функция 0eh прерывания 10h)
; В графических режимах не работает

; APS revised
; APS ! Из сегмента данных печатает строку
; APS !         по const адресу str

sayr    macro   str
        mov     si,offset str
        call    sayr_proc
endm    sayr
;#############################################################################
                locals                  ; Разрешить локальные метки
                jumps                   ; Расширять короткие переходы

                .model  tiny
                .code

                org     100h            ;?????????????????????????????
Begin:
                jmp     short start
                db      90h
OEM             db      'KERNEL',0,0
SectSize        dw      2       ;512     ;?! 2
ClustSiz        db      2
ResSecs         dw      1
FatCnt          db      2
RootSiz         dw      112
TotSecs         dw      0b40h   ;720     ;?! 0b40h
Media           db      0f0h    ;0fdh    ;?! f0
FatSize         dw      9       ;2      ;?! 9
TrkSecs         dw      18      ;9      ;?! 12h
HeadCnt         dw      2
HidnSec         dd      0
BigNo           dd      0               ; Big total no of sectors
                dw      0               ; Physical drive no
                db      9               ; Extended boot signature
FATaddr         dw      0,0             ; ! internal vars
StartClu        dw      0               ; !
Lbl             db      'No_Kernel',0,0 ; Volume label
                db      'FAT12 '        ; File system ID
RootSize        dw      '  '            ; use 2 last bytes of File system id

SectSiz         dw      512     ;?! 2

OldVec          dw      2 dup (0)

start:
        ; Table 11 bytes - drive param
        ; CS:IP = 0:7C00 SS=30 SP=F6 DS=0 ES=0 BX=7C00 Flags=0206 (DF==0)
        ; 0:0 - 0:100 - 1-я половина таблицы прерываний
        ; Стек: c 30:F6 (0:3F6) растет к 0
        ; ROM BIOS area 0:400-0:500
        ; 0:500 - 0:10500 (1000:0500) - здесь потом будет ядро
        ; Перезапись boot'а в 1000:0500 (1050:0)
                mov     si,bx           ; si <- 7C00h

                mov     cx,NewSeg
                mov     ES,cx

                mov     cx,512

                xor     di,di           ; es:di <- 1050h:0
                rep     movsb           ; 0:7C00h -> 1050h:0

                chr1    '1'             ; Indicate Ok
                db      0eah            ; JMP long l_fork (in child process)
                dw      l_fork+off,NewSeg
l_fork:
                push    es
                pop     ds

                XOR     AX,AX
                mov     dl,Drive
                int     13h     ; Disk  dl=drive ah=func 00h
                                ;  reset disk, al=return status
                chr1    '2'

loc_3:
                sayr    Ident+off       ; 'P'

                mov     al,byte ptr FatCnt+off  ; al <- количество FATов
                cbw                             ; ax <- количество FATов
                mov     cx,word ptr FatSize+off ; cx <- размер FAT
                mul     cx                      ; (dx:)ax <- суммарный
                                                ;          размер FATов
                                                ;            (dx = 0)
                add     ax,word ptr ResSecs+off ; ax <- FATы + boot
                                                ; ax - Root Beginning Sector
                mov     word ptr StartClu+off,ax
                                                ;
                PUSH    ax

                mov     ax,word ptr RootSiz+off
                mov     cx,32
                mul     cx

                mov     cx,word ptr SectSiz+off
; APS№ ▄▄▄▄▄
                dec     ax              ; 1-й шаг округления
; APS№ ^^^^^
                div     cx                      ; dx:ax / cx -> ax; mod -> dx
                                                ; ax - Root Size in Sectors
; APS№ ▄▄▄▄▄
                inc     ax              ; 2-й шаг округления
; APS№ ^^^^^

                add     word ptr StartClu+off,ax        ; Получили # 1-го
                                                        ;   клустера
                mov     cx,ax
                mov     word ptr RootSize+off,ax       ; Размер корня в секторах
                POP     ax      ; ax - Root Bg Sec
@@loop:
                                ; load root directory
                push    cx
                push    ax
                mov     bx,600h ; 1000h *******************
                mov     ES,bx
                mov     bx,Buff
                mov     dl,Drive
                call    SecRead         ; Читаем один сектор
                jc      l_err

                mov     cx,16   ; directory entries in 1 sector
                                ; ДЛЯ СТАНДАРТНОГО СЕКТОРА!!!!!!
@@l:            push    cx

                mov     cx,6
                mov     di,bx
                mov     si,offset OEM+off
                rep     cmpsb
                je      l_found

                pop     cx
                add     bx,32
                loop    @@l

                pop     ax
                inc     ax

                pop     cx
                loop    @@loop          ;Догрузить сектор и опять искать
        ; end of load root dir

;       inkey2

l_err:
        sayr    Lbl+off

Vis:    jmp     short Vis
;*****************************************************************************
; APS revised
sayr_proc       proc    ; Ver 0.0.1 9-Dec-93
                        ; Процедура вывода строки при помощи функций BIOS
                        ; Вход: DS:SI - ASCIIZ строка.
                        ; NO REG SAVED !!!
                        ; В графических режимах не работает
; APS№ ▄▄▄▄▄
        cld
; APS№ ▀▀▀▀▀
sayr_l1:
        lodsb
        or      al,al
        jz      sayr_ret
        mov     ah,0eh
        int     10h
        jmp     short sayr_l1
sayr_ret:
        ret
sayr_proc       endp
;*****************************************************************************
Ident   db      'P',0
Ver     db      'I',0

l_found:
        pop     cx
        sayr    Ver+off
;       jmp     Vis
                                        ; Found kernel
                                        ; bx - kernel dir record
                                        ; word ptr [bx+1ah] - 1st cluster
        mov     ax,word ptr es:[bx+1ah] ; ax - 1st cluster
                                        ; LoadFAT
        PUSH    ax                      ; save ax with 1st clu no

        mov     bx,Buff
        add     bx,word ptr SectSiz+off ; Addr buf for FAT
        mov     word ptr FATaddr+off,bx ; Сохранили адрес буфера.
        mov     word ptr FATaddr+2+off,es

        mov     ax,word ptr ResSecs+off ; 1-й сектор FAT
;        mov     cx,word ptr FatSize+off ; Сколько всего в ней секторов
        MOV     CX,2            ; ПОКА ДЛЯ ОТЛАДКИ

@@l:    mov     dl,Drive
        call    SecRead
        jc      l_err
        add     bx,word ptr SectSiz+off ; Увеличить буфер на размер сектора
        loop    @@l

        mov     al,byte ptr ClustSiz+off
        cbw
        mov     dx,word ptr SectSiz+off
        mul     dx
        mov     cx,ax   ; Cluster Size in Bytes

;!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        xor     ax,ax
        mov     es,ax
;!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        POP     ax      ; restore ax (1st clu of KERNEL)


        mov     bx,LoadAddr

        MOV     BX,1100h
        MOV     ES,BX
        XOR     BX,BX

l2:     mov     dl,Drive
        call    CluRead
        add     bx,cx
;        cmp     bx,off         ; ОЙ, ШО ЭТО???
;        jae     l3             ;
        call    NextClu ; set ax
        jnc     l2



        DB      0EAH
        DW      000H,1100H


        db      0eah    ; JMP PSPSeg:100
        dw      100h,PSPSeg

l3:     sayr    Lbl+1+off
l4:     jmp     l4
;ssize  db      'SIZE',0
;****************************************************************************
NextClu proc    ; Input: ax - cluster no
                ; Output: ax - next cluster from FAT
                ; cf=1 if EOF
                ; Use global var:
                ; FATaddr               dw      0
                ; REGS SAVED !

        push    bx
        push    cx
        push    dx
        push    si
        push    di
        push    es

        mov     di,ax
        mov     bx,3
        mul     bx      ; dx:ax := ax * bx
        shr     ax,1    ; ax := ax /2
        ; ax - addr in FAT
        mov     si,ax
        les     bx,dword ptr FATaddr+off
        mov     ax,word ptr es:[bx+si]
        test    di,1
        jz      @@1
        mov     cl,4
        shr     ax,cl
;       jmp     @@2
@@1:    and     ax,0fffh
@@2:
;        cmp     ax,word ptr TotSecs+off   ; ?!!!!!
        cmp     ax,0ff8h
        jae      @@cf

        clc
@@ret:
        pop     es
        pop     di
        pop     si
        pop     dx
        pop     cx
        pop     bx

        ret
@@cf:   stc
        jmp     @@ret
NextClu endp
;****************************************************************************
SecRead proc    ; ver 0.0.1   3-Nov-94 (for boot)

                ; Read absolute sectors
                ; Input: ax - abs sec number
                ;        dl - drive (for int 13h Fn=2)
                ;        ES:bx - buffer
                ;        BP - autobase register
                ;        DS - data segment reg

                ; Use global variables (based on BP!):
                ; word ptr HeadCnt+off
                ; word ptr TrkSecs+off

                ; Output: cf=1 if error
                ; ALL REGS SAVED !!!

; APS revised

        push    ax
        push    cx
        push    dx
        push    si
        push    di

        mov     si,bx   ; save bx & dx
        mov     di,dx

        PUSH    ax

         mov     ax,word ptr HeadCnt+off
         mov     cx,word ptr TrkSecs+off
         mul     cx     ; dx:ax := HeadCnt*TrkSecs т.е. Секторов на один
                        ;                       цилиндр по всем головкам
                        ;  => dx=0
         mov     bx,ax

        POP     ax
        xor     dx,dx   ; dx:ax - Abs Sec No
        div     bx      ; ax - Trk; dx - mod
                        ;  Ax - Дорожка, на которой находится нужный сектор

        mov     bx,ax   ; bx - track no

        mov     ax,dx
        xor     dx,dx   ; dx:ax - module
        div     cx      ; dx:ax / TrkSecs; ax - Head; dx - Sec on Head-1
        inc     dx      ; dx - Sec on Head

        mov     cx,5
l_loop:
        push    cx

        mov     cl,dl   ; sector
        mov     ch,bl   ; track ; Warning: track < 255 !!!
        mov     bx,si   ; Restore bx - offset for buff
        mov     dx,di   ; Restore dl - drive no
        mov     dh,al   ; head
        mov     ax,0201h; Fn=02, Read 1 sector
        int     13h
        jnc     l_break

        pop     cx
        loop    l_loop
        jmp     short l_ret
l_break:
        pop     cx
l_ret:
        pop     di
        pop     si
        pop     dx
        pop     cx
        pop     ax

        ret
SecRead endp

;****************************************************************************

CluRead proc    ; Read cluster
                ; Input: ax - cluster number
                ;        dl - drive (for int 13h Fn=2)
                ;        ES:bx - buffer
                ;        BP - autobase register
                ;        DS - data segment reg

                ; Use global variables (based on BP!):
                ; StartClu      dw      0       ; Number sector of 1st cluster
                ; ClustSiz      db      2
                ; SectSiz       dw

                ; ALL REGS SAVED !!!

; APS revised

        push    ax
        push    bx
        push    cx

        ; Sector = StartClu + ((Clu-2) * ClustSiz)
        PUSH    bx
        PUSH    dx
        dec     ax
        dec     ax
        mov     bx,ax
        mov     al,byte ptr ClustSiz+off
        cbw
        mov     cx,ax
        mul     bx
        add     ax,word ptr StartClu+off
        ; ax - sector
        POP     dx      ; Restore dl
        POP     bx
        ; cx - ClustSiz
@@l:
        call    SecRead
        jc      l_err
        add     bx,word ptr SectSiz+off
        inc     ax
        loop    @@l

        pop     cx
        pop     bx
        pop     ax

        ret
CluRead endp
;*****************************************************************************
;*****************************************************************************
;*****************************************************************************
        end     Begin
