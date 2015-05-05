        PAGE    ,79
        jumps

; HDD Boot sector ver. 0.0.0.0 2-Feb-98 from Proolix operating system
; При отладке бута учитывались многочисленные полезные замечания, которые
; сделал Алекс Семеняка и Юрий Белотицкий, bela@interfax.kharkov.ua

off     equ 7C00h-100h  ; Смещение, связанное с тем, что во время компиляции
                        ; адресом начала является 100h, а во время
                        ; выполнения 7C00

PSPSeg          equ     3050h           ; Сегментный адрес, куда будет
                                        ; загружен бутменеджер
                                        ; (точнее, его PSP)

FATseg          equ     7E0h

; Макрос. Вывод одного символа через ROM BIOS.
chr    macro   sym
        ; Not worked in graph mode (registr bl - background color!)
        ; NO SAVED REGS !!!
        mov     ax,0e00h+sym
        int     10h
        endm    chr

                        locals

                        .model  tiny
                        .code
                        org     100h

Begin:                  jmp     start
; Таблица параметров диска
                        db      90h     ; NOP
;                       '12345678'
OEM                     db      'Proolix '
SectSiz                 dw      512
ClustSiz                db      4
ResSecs                 dw      1
FatCnt                  db      2
RootSiz                 dw      512
TotSecs                 dw      0
Media                   db      0f8h
FatSize                 dw     43
TrkSecs                 dw     50
HeadCnt                 dw      2
HidnSec                 dd  40200
BigNo                   dd      0       ; Big total no of sectors
                        db      0       ; Head No
FirstClu                label   word
DriveNo_                db      80h     ; Physical drive no
                        db      29h     ; Extended boot signature
StartCluHi              dw      0       ; dd Volume serial no
StartCluLo              dw      0
Lbl                     db      'BOOT       '   ; Volume label
MaxClusters             label   word
                        db      'FAT16 ' ; File system ID
RootSize                dw      '  '; use 2 last bytes of File system id
OldVec                  dw      2 dup (0)
Sector_                 db      0
HeadNo                  db      0
TrackNo_                dw      0
RootSectLow             dw      0
RootSectHi              dw      0

start:

; Значения регистров после загрузки boot-сектора
; CS:IP = 0:7C00 SS=30 SP=F6 ES=0

; 0:0 - 0:200 - таблица прерываний
; Стек: c 30:F6 (0:3F6) растет к 0 (0:200-0:400 - стек)
; ROM BIOS area 0:400-0:500

;                mov     dl,80h
;                int     13h     ; Disk  dl=drive ah=func 00h
;                                ; reset disk, al=return status
;                jc      l_err

loc_3:
        push    CS
        pop     DS
        push    CS
        pop     ES
;        chr    '0'
        ; Вычисления
        mov     al,byte ptr FatCnt+off
        cbw
        mul     word ptr FatSize+off            ; FatCnt * FatSize -> dx:ax
        add     ax,word ptr HidnSec+off         ; low
        adc     dx,word ptr HidnSec+2+off       ; high
        add     ax,word ptr ResSecs+off
        adc     dx,0
        ; dx:ax - Root 1st Sector
        mov     RootSectLow+off,ax
        mov     RootSectHi+off,dx         ; Сохраняем

; обьем корня в секторах=(RootSiz*32)/512=RootSiz/16;

        mov     word ptr StartCluHi+off,dx
        mov     word ptr StartCluLo+off,ax

;  MaxClusters=(unsigned int)((MaxSectors-DataStart)/CluSize+1);
        mov     ax,word ptr BigNo+off
        mov     dx,word ptr BigNo+2+off
        add     ax,word ptr HidnSec+off         ; low
        adc     dx,word ptr HidnSec+2+off       ; high
        sub     ax,word ptr StartCluLo+off
        sbb     dx,word ptr StartCluHi+off
        mov     cl,ClustSiz+off
        xor     ch,ch
        div     cx
        inc     ax
        mov     MaxClusters+off,ax

        mov     ax,word ptr RootSiz+off         ; dx:ax - объем корня в байтах
        cwd
        mov     cx,16

        div     cx              ; ax <- объем корня в секторах
                                ; dx <- остаток
        or      dx,dx           ; добавление последнего неполного сектора
        jz      l_zero
        inc     ax
l_zero:
        ; ax - объем корня в секторах

        add     word ptr StartCluLo+off,ax
        adc     word ptr StartCluLo+off,0

        ; Загрузка корневого каталога посекторно и посекторный поиск в нем
        ; имени ядра

        mov     cx,ax      ; - объем корня в секторах
        mov     ax,RootSectLow+off
        mov     dx,RootSectHi+off
l_loop_1:
        mov     bx,offset Buff+off
        push    cx
        push    ax
        push    dx

        call    ReadSector         ; read sector (dx:ax)-> ES:bx
        jc      l_err

        ; Поиск ядра в корне
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

        pop     dx
        pop     ax
        pop     cx

        inc     ax
        adc     dx,0
        loop    l_loop_1
l_err:
        chr    'E'
Vis:    jmp     short Vis

l_found:        ; HАШЛИ !
;        chr    'O'

        ; Found kernel
        ; bx - kernel dir record
        ; word ptr [bx+1ah] - 1st cluster
        mov     ax,word ptr [bx+1ah]    ; ax - 1st cluster

        mov     FirstClu+off,ax

        mov     ax,word ptr HidnSec+off
        mov     dx,word ptr HidnSec+2+off
        add     ax,word ptr ResSecs+off
        adc     dx,0
        ; dx:ax - 1st FAT sector

        mov     bx,FATseg
        mov     ES,bx   ; ES=FATseg
        xor     bx,bx
        mov     cx,word ptr FatSize+off

        ; Загрузка ядра покластерно (по цепочке из FATа)

        ; Загрузка FAT
@@l:    call    ReadSector     ; read sec dx:ax -> ES:bx
        jc      l_err

        mov     si,ES
        add     si,20h
        mov     ES,si   ; ES+=20h

        inc     ax      ; Next sector!
        adc     dx,0

        loop    @@l

        mov     al,byte ptr ClustSiz+off
        cbw
        mov     dx,word ptr SectSiz+off
        mul     dx
        mov     cx,ax   ; Cluster Size in Bytes

        mov     ax,FirstClu+off
        mov     bx,PSPSeg+10h
        mov     ES,bx   ; ES=PSPSeg+10h
        xor     bx,bx
l2:     call    CluRead
;          push    ax
;          chr    '.'
;          pop     ax
        add     bx,cx
        call    NextClu ; set ax
        jnc     l2

;        chr    'X'

        ; EXEC!         Запуск ядра
;       *(int far *)MK_FP(PSP,2)=0x9FFF; /* для LZEXE */
        mov     ax,PSPSeg
        mov     DS,ax
        mov     ES,ax
        cli
        mov     SS,ax
        mov     SP,0fffeh
        sti

        db      0eah    ; JMP PSPSeg:100
        dw      100h,PSPSeg

CluRead proc    ; Read cluster
                ; Input: ax - cluster number
                ;        ES:bx - buffer
                ;        DS - data segment reg

                ; Use global variables:
                ; StartCluHi:StartCluLo - Number sector of 1st cluster
                ; ClustSiz      db      2
                ; SectSiz       dw

                ; ALL REGS SAVED !!!

        push    ax
        push    bx
        push    cx
        push    dx

        ; Sector = StartClu + ((Clu-2) * ClustSiz)
        PUSH    bx
        dec     ax      ; ax:=ax-2
        dec     ax      ;
        mov     bx,ax
        mov     al,byte ptr ClustSiz+off
        cbw
        mov     cx,ax
        mul     bx
        add     ax,word ptr StartCluLo+off
        adc     dx,word ptr StartCluHi+off
        POP     bx
        ; dx:ax - sector
@@lll:  call    ReadSector
        jc      l_err
        inc     ax
        adc     dx,0
        add     bx,512
        loop    @@lll

        pop     dx
        pop     cx
        pop     bx
        pop     ax
        ret
CluRead endp

ifdef FAT12
include fat12.asm
else
include fat16.asm
endif
;▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
;  Читает 1 сектор dx:ax в es:bx. CF, если ошибка.
ReadSector      proc    near

                push    ax
                push    cx
                push    dx

; Заполняет внутренние переменные Sector_, HeadNo и TrackNo_ значениями,
; соответствующими абс. сектору dx:ax.
                div     word ptr TrkSecs+off ; dx:ax / TrkSecs -> (dx,ax)
                inc     dl                   ; dl - # сектора
                mov     Sector_+off,dl
                xor     dx,dx                ; dx=0
                div     word ptr HeadCnt+off
                mov     HeadNo+off,dl            ; # стороны
                mov     TrackNo_+off,ax          ; # дорожки

                mov     cx,10

@@loop:         PUSH    CX
                mov     dx,TrackNo_+off ; dx=TrackNo_
                mov     cl,6            ; cl=6
                shl     dh,cl           ; dh=tt000000 B
                or      dh,Sector_+off  ; dh=ttssssss B
                mov     cx,dx           ; cx=ttsssssstttttttt
                xchg    ch,cl           ; cx=ttttttttttssssss
                mov     dl,80h          ; drive
                mov     dh,HeadNo+off
                mov     ax,0201h
                int     13h     ;  read sectors to memory es:bx
                                ;  al=#,ch=cyl,cl=sectr,dh=head
                jnc     @@ok
                POP     CX
                loop    @@loop
                stc
@@ret:          pop     dx
                pop     cx
                pop     ax
                retn
@@ok:
                POP     CX
                jmp     @@ret
ReadSector      endp
;▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
Buff    label   byte
        end     Begin
