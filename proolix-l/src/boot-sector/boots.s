# 1 "boots.S"
# 1 "<built-in>"
# 1 "<command line>"
# 1 "boots.S"
# Proolix-l boot sector for diskette 1.44 M 3"
# for install boot sector to diskette /dev/fd0 usage 'make install'


# 30 "boots.S"
off     =     0x7c00

Drive   =     0       # Загрузочный диск A: (FDD 0)

stage2_seg          =     0x3050       # Сегментный адрес, куда будет
                                       # загружен stage2

FATaddr         =     Buff

        .code16
	.text
	
.global _start
_start:

Begin:          jmp     start
# Таблица параметров диска
                 .byte      0x90     # NOP
OEM:             .ascii     "Proolix "
SectSiz:         .word      512
ClustSiz:        .byte      2
ResSecs:         .word      1
FatCnt:          .byte      2
RootSiz:         .word      112
TotSecs:         .word      720
Media:           .byte      0xfd
FatSize:         .word      2
TrkSecs:         .word      9
HeadCnt:         .word      2
HidnSec:         .word      0,0
BigNo:           .word      0,0       # Big total no of sectors
                .byte      0       # Head No
                .byte      0       # Physical drive no
                .byte      0x29    # Extended boot signature
                # .dword     1       # Volume serial no
# FATaddr:        label   word    # !
                 .word      0       # ! internal vars
StartClu:        .word      0       # !
Lbl:             .ascii      "BOOT       "   # Volume label
                 .ascii      "FAT12 " # File system ID
RootSize:        .ascii	     "  " # use 2 last bytes of File system id
OldVec:          .word      0,0

start:
#        pushw   %CS
#        popw    %DS
#        sayr    OEM+off
# Значения регистров после загрузки boot-сектора
# CS:IP = 0:7C00 SS=30 SP=F6 ES=0


# Стек: c 30:F6 (0:3F6) растет к 0 (0:200-0:400 - стек)
# ROM BIOS area 0:400-0:500

#       cli
        # Модификация diskette param table (взято из бута MSDOS 6.0)

# нужен ли в нижеследующей команде префикс ES: ?
	lds  %es:(0x78),%si # DS:si - vector diskette param table


# 105 "boots.S"
                movb    $0xf,9(%si)  # head settle := f 
                movw    TrkSecs+off,%cx # TrkSecs
                movb    %cl,6(%si) # Data Transfer Length
#               sti
                movb    $Drive,%dl
                xorw    %ax,%ax
                int     $0x13   # Disk  %dl=drive %ah=func 00
                                # reset disk, %al=return status
                jc      l_err

        pushw   %CS
        popw    %ES

        pushw   %CS
        popw    %DS

        # Вычисления
        movb    FatCnt+off,%al
        cbw
        movw    FatSize+off,%cx
	mulw    %cx      # %ax*%cx -> %dx:%ax
        addw    ResSecs+off,%ax
        # %ax - Root Beginning Sector
        movw    %ax,StartClu+off
        PUSHW   %ax

        movw    RootSiz+off,%ax
        movw    $32,%cx
        mulw    %cx

        movw    SectSiz+off,%cx
        divw    %cx      # %dx:%ax / %cx -> %ax; mod -> %dx
        orw     %dx,%dx   # Если остаток ненулевой, то прибавляем еще один
        jz      l_ll    # сектор (последний неполный сектор root'а)
        inc     %ax
l_ll:
        # %ax - Root Size in Sectors
        addw    %ax,StartClu+off
        movw    %ax,%cx
        movw    %ax,RootSize+off
        POPW    %ax      # %ax - Root Bg Sec
loop1:  # Загрузка корневого каталога посекторно и посекторный поиск в нем
        # имени ядра
        pushw   %cx

        pushw   %ax
        movw    $Buff+off,%bx
        movb    $Drive,%dl
        pushw   %ax
        call    SecRead
        jc      l_err
        popw    %ax

#               pushw   %ax
#               CHR1    'R'
#               popw    %ax

        movw    $16,%cx   # root entryes in 1 sector
loop2:          pushw   %cx

                movw    $11,%cx
                movw    %bx,%di
		movw    $Lbl+off,%si
                rep     cmpsb
                je      l_found

                popw    %cx
                addw    $32,%bx
                loop    loop2

        popw    %ax
        incw    %ax

        popw    %cx
        loop    loop1
        # end of load root dir
        # HЕ HАШЛИ ЯДРА

l_err:
#       CHR1    'E'
        call    ohw
        xorb    %ah,%ah
        int     $0x16
        int     $0x19
#Vis:    jmp     short Vis

sayr_proc: # proc       Ver 0.0.1 9-Dec-93
                        # Процедура вывода строки при помощи функций BIOS
                        # Вход: %DS:%si - ASCIIZ строка.
                        # NO REG SAVED !!!
# В графических режимах не работает
#       cld
sayr_l1:
        lodsb
        orb     %al,%al
        jz      sayrret
        movb    $0x0e,%ah
        int     $0x10
        jmp     sayr_l1
sayrret:
        ret
# sayr_proc       endp

l_found:        # HАШЛИ !
        popw    %cx
        # Found kernel
        # %bx - kernel dir record
        # word ptr (%bx+0x1a) - 1st cluster
        movw    0x1a(%bx),%ax    # %ax - 1st cluster

        # Загрузка FAT в ОЗУ
        PUSHW   %ax      # save %ax with 1st clu no

        movw    ResSecs+off,%ax
        movw    $FATaddr+off,%bx
        movw    FatSize+off,%cx

        # Загрузка ядра покластерно (по цепочке из FATа)
        # Загрузка FAT
loop3:  movb    $Drive,%dl
        pushw   %ax
        call    SecRead
        jc      l_err
        popw    %ax
        addw    $512,%bx
        incw    %ax      # Next sector!

#       pushw   %ax
#       CHR1    'F'
#       popw    %ax

        loop    loop3

        movb    ClustSiz+off,%al
        cbw
        movw    SectSiz+off,%dx
        mul     %dx
        movw    %ax,%cx   # Cluster Size in Bytes

        POPW    %ax      # restore %ax (1st clu no)
        movw    $stage2_seg,%bx
        movw    %bx,%ES
        xorw    %bx,%bx
l2:     movb    $Drive,%dl
        call    CluRead
          pushw   %ax
	  movw $0x0e00+176,%ax
	  int  $0x10
          popw    %ax
        addw    %cx,%bx
        call    NextClu # set %ax
        jnc     l2

        # EXEC!       
	
#       sayr    OEM+off

        movw    $stage2_seg,%ax
        movw    %ax,%DS
        cli
        movw    %ax,%SS
        movw    $0xfffe,%SP
        sti

        .byte      0xea    # JMP stage2_seg:0000
        .word      0x0000,stage2_seg

NextClu: # proc # Input: %ax - cluster no
                # Output: %ax - next cluster from FAT
                # cf=1 if EOF
                # Use global var:
                # FATaddr               .word      0
                # REGS SAVED !

        pushw    %bx
        pushw    %cx
        pushw    %dx
        pushw    %si
        pushw    %di

        movw    %ax,%di
        movw    $3,%bx
        mul     %bx      # %dx:%ax := %ax * %bx
        shrw    $1,%ax   # %ax := %ax /2
        # %ax - addr in FAT
        movw    %ax,%si
        movw    $FATaddr+off,%bx
        movw    (%bx,%si),%ax
        testw   $1,%di
        jz      lbl5
        movb    $4,%cl
        shrw    %cl,%ax
        jmp     lbl6
lbl5:   andw    $0xfff,%ax
lbl6:
#       cmpw    $0xfff,%ax
#       je      l_cf
#       cmpw    $0xff7,%ax
#       je      l_cf
        cmpw    TotSecs+off,%ax
        ja      l_cf # $$$

        clc
l_ret1:
        popw     %di
        popw     %si
        popw     %dx
        popw     %cx
        popw     %bx

        ret
l_cf:   stc
        jmp     l_ret1
# NextClu endp

SecRead:        # Read absolute sectors
                # Input: %ax - abs sec number
                #        %dl - drive (for int 0x13 Fn=2)
                #        ES:%bx - buffer

                # Use global variables:
                # word ptr HeadCnt+off
                # word ptr TrkSecs+off

                # Output: cf=1 if error
                # NO ALL REGS SAVED !!!

#       pushw    %ax
        pushw    %bx
        pushw    %cx
        pushw    %dx
        pushw    %si
        pushw    %di
 
        movw    %bx,%si   # save %bx & %dx
        movw    %dx,%di

        PUSHW    %ax

        movw    HeadCnt+off,%ax
        movw    TrkSecs+off,%cx
        mul     %cx      # %dx:%ax := HeadCnt * TrkSecs
        movw    %ax,%bx  # %bx = HeadCnt * TrkSecs

        POPW     %ax
        xorw     %dx,%dx   # %dx:%ax - Abs Sec No
        div     %bx # %ax=Track=AbsSec/(HeadCnt*TrkSecs); %dx - mod (Sec on Cyl)

        movw    %ax,%bx   # %bx - track no

        movw    %dx,%ax
        xorw    %dx,%dx   # %dx:%ax - module (Sec on Cyl)
        div     %cx # Head=[%dx:%ax]/TrkSecs;%ax - Head; mod (%dx) - Sec on Head-1
        incw    %dx # %dx - Sec on Head

#        movw    $1,%cx
#l_loop:
#        pushw    %cx

        movb     %dl,%cl    # sector
        movb     %bl,%ch    # track ; Warning: track < 255 !!!
        movw     %si,%bx    # Restore %bx - offset for buff
        movw     %di,%dx    # Restore %dl - drive no
        movb     %al,%dh    # head
        movw     $0x201,%ax # Fn=02, Read 1 sector
        int      $0x13
#        jnc     l_break

#        popw     %cx
#        loop    l_loop
#        jmp     l_ret
#l_break:
#        popw     %cx
l_ret:
        popw     %di
        popw     %si
        popw     %dx
        popw     %cx
        popw     %bx
#       popw     %ax

        ret
# SecRead endp

CluRead: # proc    # Read cluster
                # Input: %ax - cluster number
                #        %dl - drive (for int 0x13 Fn=2)
                #        ES:%bx - buffer
                #        DS - data segment reg

                # Use global variables:
                # StartClu      .word      0       # Number sector of 1st cluster
                # ClustSiz      .byte      2
                # SectSiz       .word

                # ALL REGS SAVED !!!

        pushw    %ax
        pushw    %bx
        pushw    %cx

        # Sector = StartClu + ((Clu-2) * ClustSiz)
        PUSHW    %bx
        PUSHW    %dx
        decw     %ax      # %ax:=%ax-2
        decw     %ax      #
        movw    %ax,%bx
        movb    ClustSiz+off,%al
        cbw
        movw    %ax,%cx
        mul     %bx
        addw    StartClu+off,%ax
        # %ax - sector
        POPW     %dx      # Restore %dl
        POPW     %bx
        # %cx - ClustSiz
loop4:
        pushw    %ax
        call    SecRead
          # jc      l_err
	  jnc	obhod
	  jmp	l_err
obhod:	  
        popw     %ax
        addw    SectSiz+off,%bx
        incw     %ax
        loop    loop4

        popw     %cx
        popw     %bx
        popw     %ax

        ret
# CluRead endp

ohw: #     proc

#       Вывод слова в HEX-виде. Вход: слово в %ax.
#       Все регистры сохраняются.
#       Вызывает подпрограмму ohb

        pushw    %ax      # Сохр. ради %al.
        movb     %ah,%al
        call    ohb
        popw     %ax              # Восст. %al.
        call    ohb
        ret
# ohw     endp
ohb: #     proc

# Procedure output hex byte Ver 0.1.1 6 Dec 93 via BIOS
# Input: %al - byte
# All regs. reserved ;)

# Not worked in graph mode. bl - bg color !!!

        pushw    %ax
        pushw    %cx
        pushw    %dx

        movb     %al,%dl
        movb     $4,%cl
        shrb     %cl,%al
        call    ohb1

        movb    %dl,%al
        andb    $0xf,%al
        call    ohb1

        popw     %dx
        popw     %cx
        popw     %ax

        ret

# ohb     endp

ohb1: #    proc    # Regs not saved !!!
        pushw    %ax

        cmpb     $9,%al
        ja      l_1    # %al > 9
        # %al <= 9
        addb     $0x30,%al	# addb '0',%al
        jmp     l_out

l_1:   addb     $0x37,%al #  addb    'A'-10,%al 
l_out: movb     $0xe,%ah
        int     $0x10

        popw     %ax

        ret
# ohb1    endp

Buff:
