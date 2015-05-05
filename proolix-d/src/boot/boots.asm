        PAGE    ,79
        jumps

; FDD Boot sector ver. 0.0.4.0 20-Apr-96 from Proolix operating system
; Copyright (C) Serge Pustovoitoff, 1993-1996
; При отладке бута учитывались многочисленные полезные замечания, которые
; сделал Алекс Семеняка и Юрий Белотицкий (bela@padco.kharkov.ua)

; History in stack order
; 0.0.4.0 20-Apr-96 загружаемый файл /boot может быть сжат pklite'ом
; 0.0.3.1 24-Mar-96 replace chr1 '.' -> char1  176
; 0.0.3.1 12-Mar-96 PSPSeg = 3050h
; 0.0.3.0 25-Feb-96 PSPSeg = 1050h
; 0.0.3.0 10-Jan-96 - загружаем не /kernel, а /boot (см. файл history,
; запись 0.0.1.4 10-Jan-96
; 0.0.2.3 16-Sep-95
; 0.0.2.2 15-Sep-95 - разветвление на FDD и HDD бут сектора
; 0.0.2.1 25-Feb-95 - до этого история не велась... (см. также общую историю
; в файле kernel/history.doc

; Смещение, связанное с тем, что во время компиляции
; адресом начала является 100h, а во время
; выполнения 7c00h
off     equ     7c00h-100h

Drive   equ     0       ; Загрузочный диск A: (FDD 0)

PSPSeg          equ     3050h           ; Сегментный адрес, куда будет
                                        ; загружен /boot (точнее, его PSP)

FATaddr         equ     Buff

; Макрос. Вывод одного символа через ROM BIOS.
chr1    macro   sym
        ; Not worked in graph mode (registr bl - background color!)
        ; NO SAVE REGS !!!
        mov     ax,0e00h+sym
        int     10h
        endm    chr1

sayr    macro   str
; NO SAVE REGS !!!

; Макрокоманда, аналогичная макрокоманде say. Только say использует функцию 9h
; ДОС, а sayr - функцию ROM BIOS (функция 0eh прерывания 10h)
; В графических режимах не работает

        mov     si,offset str
        call    sayr_proc
        endm    sayr

        locals

        .model  tiny
        .code
        org     100h

Begin:          jmp     short start
; Таблица параметров диска
                db      90h     ; NOP
;                       '12345678'
OEM             db      'Proolix '
SectSiz         dw      512
ClustSiz        db      2
ResSecs         dw      1
FatCnt          db      2
RootSiz         dw      112
TotSecs         dw      720
Media           db      0fdh
FatSize         dw      2
TrkSecs         dw      9
HeadCnt         dw      2
HidnSec         dd      0
BigNo           dd      0       ; Big total no of sectors
                db      0       ; Head No
                db      0h      ; Physical drive no
                db      29h     ; Extended boot signature
                ;dd     1       ; Volume serial no
;FATaddr        label   word    ; !
                dw      0       ; ! internal vars
StartClu        dw      0       ; !
Lbl             db      'BOOT       '   ; Volume label
                db      'FAT12 ' ; File system ID
RootSize        dw      '  '; use 2 last bytes of File system id
OldVec          dw      2 dup (0)

start:
;        push    CS
;        pop     DS
;        sayr    OEM+off
; Значения регистров после загрузки boot-сектора
; CS:IP = 0:7C00 SS=30 SP=F6 ES=0

; 0:0 - 0:200 - таблица прерываний
; Стек: c 30:F6 (0:3F6) растет к 0 (0:200-0:400 - стек)
; ROM BIOS area 0:400-0:500

;       cli
        ; Модификация diskette param table (взято из бута MSDOS 6.0)

        ; нужен ли в нижеследующей команде префикс ES: ?
        lds  si,dword ptr ES:[78h] ; DS:si - vector diskette param table
; diskette param tble
; Bytes:
; 0 - bit 0-3 - SRT (step rate time), 4-7 head unload time
; 1 - bit 0: 1 if DMA, 2-7 head load time
; 2 - motor wait (in 55 ms)
; 3 - sec size (0 - 128, 1 - 256, 2 - 512, 3 - 1024)
; 4 - EOT (finish sector on track)
; 5 - interval length for ReadWrite
; 6 - DTL - Data Transfer Length (maximal)
; 7 - interval length for formatting
; 8 - Filler
; 9 - head settle time (in ms)
; a - motor start time (in 1/8 s)
                mov     byte ptr [si+9],0Fh  ; head settle := f
                mov     cx,TrkSecs+off ; TrkSecs
                mov     [si+6],cl ; Data Transfer Length
;               sti
                mov     dl,Drive
                xor     ax,ax
                int     13h     ; Disk  dl=drive ah=func 00h
                                ; reset disk, al=return status
                jc      l_err

        push    CS
        pop     ES

        push    CS
        pop     DS

        ; Вычисления
        mov     al,byte ptr FatCnt+off
        cbw
        mov     cx,word ptr FatSize+off
        mul     cx      ; ax*cx -> dx:ax
        add     ax,word ptr ResSecs+off
        ; ax - Root Beginning Sector
        mov     word ptr StartClu+off,ax
        PUSH    ax

        mov     ax,word ptr RootSiz+off
        mov     cx,32
        mul     cx

        mov     cx,word ptr SectSiz+off
        div     cx      ; dx:ax / cx -> ax; mod -> dx
        or      dx,dx   ; Если остаток ненулевой, то прибавляем еще один
        jz      l_ll    ; сектор (последний неполный сектор root'а)
        inc     ax
l_ll:
        ; ax - Root Size in Sectors
        add     word ptr StartClu+off,ax
        mov     cx,ax
        mov     word ptr RootSize+off,ax
        POP     ax      ; ax - Root Bg Sec
@@loop: ; Загрузка корневого каталога посекторно и посекторный поиск в нем
        ; имени ядра
        push    cx

        push    ax
        mov     bx,offset Buff+off
        mov     dl,Drive
        push    ax
        call    SecRead
        jc      l_err
        pop     ax

;               push    ax
;               chr1    'R'
;               pop     ax

        mov     cx,16   ; root entryes in 1 sector
@@l:            push    cx

                mov     cx,11
                mov     di,bx
                mov     si,offset Lbl+off
                rep     cmpsb
                je      l_found

                pop     cx
                add     bx,32
                loop    @@l

        pop     ax
        inc     ax

        pop     cx
        loop    @@loop
        ; end of load root dir
        ; HЕ HАШЛИ ЯДРА

l_err:
;        chr1    'E'
        call    ohw
        xor     ah,ah
        int     16h
        int     19h
;Vis:    jmp     short Vis

sayr_proc       proc    ; Ver 0.0.1 9-Dec-93
                        ; Процедура вывода строки при помощи функций BIOS
                        ; Вход: DS:SI - ASCIIZ строка.
                        ; NO REG SAVED !!!
; В графических режимах не работает
;       cld
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

l_found:        ; HАШЛИ !
        pop     cx
        ; Found kernel
        ; bx - kernel dir record
        ; word ptr [bx+1ah] - 1st cluster
        mov     ax,word ptr [bx+1ah]    ; ax - 1st cluster

        ; Загрузка FAT в ОЗУ
        PUSH    ax      ; save ax with 1st clu no

        mov     ax,word ptr ResSecs+off
        mov     bx,offset FATaddr+off
        mov     cx,word ptr FatSize+off

        ; Загрузка ядра покластерно (по цепочке из FATа)
        ; Загрузка FAT
@@l:    mov     dl,Drive
        push    ax
        call    SecRead
        jc      l_err
        pop     ax
        add     bx,512
        inc     ax      ; Next sector!

;       push    ax
;       chr1    'F'
;       pop     ax

        loop    @@l

        mov     al,byte ptr ClustSiz+off
        cbw
        mov     dx,word ptr SectSiz+off
        mul     dx
        mov     cx,ax   ; Cluster Size in Bytes

        POP     ax      ; restore ax (1st clu no)
        mov     bx,PSPSeg
        mov     ES,bx
        mov     bx,100h
l2:     mov     dl,Drive
        call    CluRead
          push    ax
          chr1    176
          pop     ax
        add     bx,cx
        call    NextClu ; set ax
        jnc     l2

        ; EXEC!         Запуск ядра
;        mov     ax,PSPSeg
;        mov     DS,ax
;        mov     ES,ax
;        cli
;        mov     SS,ax
;        mov     SP,0fffeh
;        sti

;       sayr    OEM+off

;       *(int far *)MK_FP(PSP,2)=0x9FFF; /* для LZEXE */
        mov     ax,PSPSeg
        mov     DS,ax
        cli
        mov     SS,ax
        mov     SP,0fffeh
        sti
;       mov     ES,ax
;       mov     word ptr ES:[2],9FFFh

        db      0eah    ; JMP PSPSeg:100
        dw      100h,PSPSeg

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

        mov     di,ax
        mov     bx,3
        mul     bx      ; dx:ax := ax * bx
        shr     ax,1    ; ax := ax /2
        ; ax - addr in FAT
        mov     si,ax
        mov     bx,offset FATaddr+off
        mov     ax,word ptr [bx+si]
        test    di,1
        jz      @@1
        mov     cl,4
        shr     ax,cl
        jmp     @@2
@@1:    and     ax,0fffh
@@2:
;       cmp     ax,0fffh
;       je      @@cf
;       cmp     ax,0ff7h
;       je      @@cf
        cmp     ax,word ptr TotSecs+off
        ja      @@cf

        clc
@@ret:
        pop     di
        pop     si
        pop     dx
        pop     cx
        pop     bx

        ret
@@cf:   stc
        jmp     @@ret
NextClu endp

SecRead proc    ; ver 0.0.1   3-Nov-94 (for FDD boot)

                ; Read absolute sectors
                ; Input: ax - abs sec number
                ;        dl - drive (for int 13h Fn=2)
                ;        ES:bx - buffer
                ;        DS - data segment reg

                ; Use global variables:
                ; word ptr HeadCnt+off
                ; word ptr TrkSecs+off

                ; Output: cf=1 if error
                ; NO ALL REGS SAVED !!!

;       push    ax
        push    bx
        push    cx
        push    dx
        push    si
        push    di

        mov     si,bx   ; save bx & dx
        mov     di,dx

        PUSH    ax

        mov     ax,word ptr HeadCnt+off
        mov     cx,word ptr TrkSecs+off
        mul     cx      ; dx:ax := HeadCnt * TrkSecs
        mov     bx,ax   ; bx = HeadCnt * TrkSecs

        POP     ax
        xor     dx,dx   ; dx:ax - Abs Sec No
        div     bx ; ax=Track=AbsSec/(HeadCnt*TrkSecs); dx - mod (Sec on Cyl)

        mov     bx,ax   ; bx - track no

        mov     ax,dx
        xor     dx,dx   ; dx:ax - module (Sec on Cyl)
        div     cx ; Head=[dx:ax]/TrkSecs;ax - Head; mod (dx) - Sec on Head-1
        inc     dx ; dx - Sec on Head

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
        pop     bx
;       pop     ax

        ret
SecRead endp

CluRead proc    ; Read cluster
                ; Input: ax - cluster number
                ;        dl - drive (for int 13h Fn=2)
                ;        ES:bx - buffer
                ;        DS - data segment reg

                ; Use global variables:
                ; StartClu      dw      0       ; Number sector of 1st cluster
                ; ClustSiz      db      2
                ; SectSiz       dw

                ; ALL REGS SAVED !!!

        push    ax
        push    bx
        push    cx

        ; Sector = StartClu + ((Clu-2) * ClustSiz)
        PUSH    bx
        PUSH    dx
        dec     ax      ; ax:=ax-2
        dec     ax      ;
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
        push    ax
        call    SecRead
        jc      l_err
        pop     ax
        add     bx,word ptr SectSiz+off
        inc     ax
        loop    @@l

        pop     cx
        pop     bx
        pop     ax

        ret
CluRead endp

ohw     proc

;       Вывод слова в HEX-виде. Вход: слово в ax.
;       Все регистры сохраняются.
;       Вызывает подпрограмму ohb

        push    ax      ; Сохр. ради al.
        mov     al,ah
        call    ohb
        pop     ax              ; Восст. al.
        call    ohb
        ret
ohw     endp
ohb     proc

; Procedure output hex byte Ver 0.1.1 6 Dec 93 via BIOS
; Input: AL - byte
; All regs. reserved ;)

; Not worked in graph mode. bl - bg color !!!

        push    ax
        push    cx
        push    dx

        mov     dl,al
        mov     cl,4
        shr     al,cl
        call    ohb1

        mov     al,dl
        and     al,0fh
        call    ohb1

        pop     dx
        pop     cx
        pop     ax

        ret

ohb     endp

ohb1    proc    ; Regs not saved !!!
        push    ax

        cmp     al,9
        ja      @@_1    ; al > 9
        ; al <= 9
        add     al,'0'
        jmp     @@_out

@@_1:   add     al,'A'-10
@@_out: mov     ah,0eh
        int     10h

        pop     ax

        ret
ohb1    endp

Buff    label   byte
        end     Begin
