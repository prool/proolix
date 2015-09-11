        PAGE    ,79
; Boot sector ver. 0.0.0.8  7-Nov-94 from Proolix operating system
; Copyright (C) Serge Pustovoitoff, 1993, 1994

off     equ     7c00h - 100h
Drive   equ     0
ModifyParams    equ     'Yes'

PSPSeg          equ     50h     ; 70h

LoadAddr        equ     (PSPSeg shl 4)+100h

        include macros.asm

sayr    macro   str
; NO SAVE REGS !!!

; Макрокоманда, аналогичная макрокоманде say. Только say использует функцию 9h
; ДОС, а sayr - функцию BIOS (функция 0eh прерывания 10h)
; В графических режимах не работает

        mov     si,offset str
        call    sayr_proc
        endm    sayr
        locals
        jumps

        .model  tiny
        .code
        org     100h
Begin:
                jmp     short l
                db      90h
;                       '12345678'
OEM             db      'KERNEL',0,0
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
                dw      0       ; Physical drive no
                db      9       ; Extended boot signature
                ;dd     1       ; Volume serial no
FATaddr         dw      0       ; ! internal vars
StartClu        dw      0       ; !
Lbl             db      'No_Kernel',0,0   ; Volume label
                db      'FAT12 ' ; File system ID
RootSize        dw      '  '; use 2 last bytes of File system id
OldVec          dw      2 dup (0)

l: ; Table 11 bytes - drive param
        cli
        xor     ax,ax
        mov     es,ax
                ifdef   ModifyParams
                mov     bx,78h
; вот до сих пор будет таблица в 11 байт
                lds     si,dword ptr es:[bx] ; ds:si - vector diskette param
                mov     word ptr OldVec+off,si
                mov     word ptr OldVec+off+2,ds
;               push    ds
;               push    si
;               push    ss
;               push    bx
                mov     di,offset l+off
                mov     cx,0Bh
                cld
                rep movsb ;11 bytes from diskette param tble to begin of pgm

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

                push    es
                pop     ds
                mov     byte ptr [di-2],0Fh  ; head settle := f
                mov     cx,ds:TrkSecs+off ; TrkSecs
                mov     [di-7],cl ; Data Transfer Length

                mov     [bx+2],ax
                mov     word ptr [bx],offset l+off ; new diskette table
                endif

                sti
                mov     dl,Drive
                int     13h     ; Disk  dl=drive ah=func 00h
                                ;  reset disk, al=return status
comment |
                jc      l_err
                xor     ax,ax
                cmp     TotSecs+off,ax  ; TotSecs cmp with 0
                je      loc_3
                mov     cx,TotSecs+off ; TotSecs
                mov     word ptr BigNo+off,cx ; Big total no of sec
|
loc_3:
        sayr    Ident+off
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
        ; ax - Root Size in Sectors
        add     word ptr StartClu+off,ax
        mov     cx,ax
        mov     word ptr RootSize+off,ax
        POP     ax      ; ax - Root Bg Sec
@@loop: ; load root directory
        push    cx

        push    ax
        mov     bx,offset Buff+off
        mov     dl,Drive
        call    SecRead
        jc      l_err

;       chr1    '#'
        mov     cx,16   ; root entryes in 1 sector
@@l:            push    cx

                mov     cx,6
                mov     si,bx
                mov     di,offset OEM+off
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

;       inkey2

l_err:
        sayr    Lbl+off
@@ll:   jmp     short @@ll

;       include ohb1.asm
;       include ohw.asm

l_found:
        pop     cx
        ;sayr   Ver+off
        ; Found kernel
        ; bx - kernel dir record
        ; word ptr [bx+1ah] - 1st cluster
        mov     ax,word ptr [bx+1ah]    ; ax - 1st cluster

        ;LoadFAT
        PUSH    ax      ; save ax with 1st clu no

        mov     bx,offset Buff+off
        mov     ax,word ptr RootSize+off
        mov     cx,word ptr SectSiz+off
        mul     cx
        ; ax - RootSize in bytes
        add     bx,ax   ; Addr buf for FAT
        mov     word ptr FATaddr+off,bx

        mov     ax,word ptr ResSecs+off
        mov     cx,word ptr FatSize+off

@@l:    mov     dl,Drive
        call    SecRead
        jc      l_err
        add     bx,512
        loop    @@l

        mov     al,byte ptr ClustSiz+off
        cbw
        mov     dx,word ptr SectSiz+off
        mul     dx
        mov     cx,ax   ; Cluster Size in Bytes

        POP     ax      ; restore ax (1st clu no)
        mov     bx,LoadAddr
l2:     mov     dl,Drive
        call    CluRead
        add     bx,cx
        cmp     bx,off
        jae     l3
        call    NextClu ; set ax
        jnc     l2

        ; EXEC!
;       mov     ax,PSPSeg
;       mov     DS,ax
;       mov     ES,ax
;       cli
;       mov     SS,ax
;       mov     SP,0fffeh
;       sti
        db      0eah    ; JMP PSPSeg:100
        dw      100h,PSPSeg

l3:     sayr    L_err+off
l4:     jmp     l4
;ssize  db      'SIZE',0

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
        mov     bx,word ptr FATaddr+off
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
        mul     cx      ; dx:ax := HeadCnt * TrkSecs
        mov     bx,ax

        POP     ax
        xor     dx,dx   ; dx:ax - Abs Sec No
        div     bx      ; ax - Trk; dx - mod

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
        mov     bx,si   ; Restore dl - drive no
        mov     dx,di   ; Restore bx - offset for buff
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

        push    ax
        push    bx
        push    cx

        push    bx
        mov     bl,1
        chr1    '#'
        pop     bx

        ; Sector = StartClu + ((Clu-2) * ClustSiz)
        PUSH    bx
        PUSH    dx
        sub     ax,2
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

comment |
reada1  proc    ; Regs no save !!!

        xor     ax,ax
        xor     dl,dl
        mov     bx,offset Buff+off
        call    SecRead
        jc      l_err
        sayr    Buff-base+3[bp]

        ret
reada1  endp
|
comment |
reada   proc    ; Reg no save !!!
        mov     ah,2                    ; Fn read
        mov     al,1                    ; num sec
        mov     dl,Drive                ; drive
        mov     dh,0                    ; head
        mov     cx,1                    ; cyl/sec
        mov     bx,offset Buff+off

        int     13h
        jc      l_err
        sayr    Buff-base+3[bp]
        ret
reada   endp
|
comment |
saycsip proc    ; REG SAVED
        push ax

        sayr    s0+off
        push    CS
        pop     ax
        call    ohw
        chr1    ':'
        mov     ax,Begin+off
        call    ohw

;       sayr    s_ax+off
;       pop     ax
;       push    ax
;       call    ohw

;       sayr    s_bx+off
;       mov     ax,bx
;       call    ohw

;       sayr    s_cx+off
;       mov     ax,cx
;       call    ohw

;       sayr    s_dx+off
;       mov     ax,dx
;       call    ohw

;       sayr    s_si+off
;       mov     ax,si
;       call    ohw

;       sayr    s_di+off
;       mov     ax,di
;       call    ohw

        sayr    s_ss+off
        mov     ax,SS
        call    ohw

        sayr    s_sp+off
        mov     ax,SP
        call    ohw

        sayr    s_ds+off
        mov     ax,DS
        call    ohw

        sayr    s_es+off
        mov     ax,ES
        call    ohw

;       sayr    s_bp+off
;       mov     ax,BP
;       call    ohw

        pop     ax
        ret
s0      db      13,10,"Bg CS:IP=",0
;s_ax   db      ' ax=',0
;s_bx   db      ' bx=',0
;s_cx   db      ' cx=',0
;s_dx   db      ' dx=',0
;s_si   db      ' si=',0
;s_di   db      ' di=',0
s_SS    db      ' SS=',0
s_SP    db      ' SP=',0
s_DS    db      ' DS=',0
s_ES    db      ' ES=',0
;s_bp   db      ' BP=',0
saycsip endp
|
        comment |
ints    proc    ; Reg no saved !!!
        sayr    s1+off

        mov     cx,0ffh
        mov     bp,0
l1:
        mov     al,0ffh
        sub     al,cl
        call    ohb

        chr1    ' '

        mov     ax,[bp+2]
        call    ohw

        chr1    ':'

        mov     ax,[bp]
        call    ohw

        chr1    13
        chr1    10

        add     bp,4
        test    cx,0fh
        jnz     l3
        inkey2
l3:
        loop    l1

        ret
s1      db      13,10,"ints",13,10,0
ints    endp
|
comment |
Int21   proc
        sayr    Ver+off
        iret
Int21   endp
|
comment |
dump    proc
        mov     cx,520
        xor     si,si
@@l:
        mov     ax,byte ptr Begin-base[bp+si]
        call    ohb
        inc     si
        loop    @@l
        ret
dump    endp
|
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
Ident   db      'PILO',0
;Ident   db      'Proolix by Prool. Load ',0
L_err   db      'Error',0
Buff    label   byte    ; for 512 byte space

        end     Begin
