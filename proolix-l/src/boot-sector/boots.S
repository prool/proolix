# Proolix-l boot sector for diskette 1.44 M 3"

/*
# FDD Boot sector ver. 0.0.5.1 21-Oct-2004 for OS Proolix-l
# by Serge Pustovoitoff, 1993-2004

# При отладке бута учитывались многочисленные полезные замечания, которые
# сделали Алекс Семеняка и Юрий Белотицкий

# History in stack order
# 0.0.5.1 21-Oct-2004
# 0.0.5.0 12-Oct-2004 - переделано под Linux (tasm->as)
################################################################################
# 0.0.4.0 20-Apr-96 загружаемый файл /boot может быть сжат pklite'ом
# 0.0.3.1 24-Mar-96 replace chr1 '.' -> char1  176
# 0.0.3.1 12-Mar-96 PSPSeg (stage2_seg) = 0x3050
# 0.0.3.0 25-Feb-96 PSPSeg = 0x1050
# 0.0.3.0 10-Jan-96 - загружаем не /kernel, а /boot (см. файл history,
# запись 0.0.1.4 10-Jan-96
# 0.0.2.3 16-Sep-95
# 0.0.2.2 15-Sep-95 - разветвление на FDD и HDD бут сектора
# 0.0.2.1 25-Feb-95 - до этого история не велась... (см. также общую историю
# в файле kernel/history.doc

# Смещение, связанное с тем, что во время компиляции
# адресом начала является 0, а во время
# выполнения 0x7c00
*/
off     =     0x7c00
SecSize	=	512

Drive   =     0       # Загрузочный диск A: (FDD 0)

stage2_seg          =     0x3050       # Сегментный адрес, куда будет
                                       # загружен stage2

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

# /* 0:0 - 0:200 - таблица прерываний */
# Стек: c 30:F6 (0:3F6) растет к 0 (0:200-0:400 - стек)
# ROM BIOS area 0:400-0:500

#       cli
        # Модификация diskette param table (взято из бута MSDOS 6.0)

# нужен ли в нижеследующей команде префикс ES: ?
	lds  %es:(0x78),%si # DS:si - vector diskette param table

/*
# diskette param table
# Bytes:
# 0 - bit 0-3 - SRT (step rate time), 4-7 head unload time
# 1 - bit 0: 1 if DMA, 2-7 head load time
# 2 - motor wait (in 55 ms)
# 3 - sec size (0 - 128, 1 - 256, 2 - 512, 3 - 1024)
# 4 - EOT (finish sector on track)
# 5 - interval length for ReadWrite
# 6 - DTL - Data Transfer Length (maximal)
# 7 - interval length for formatting
# 8 - Filler
# 9 - head settle time (in ms)
# a - motor start time (in 1/8 s)
*/
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

	movw	$0xe00+'1',%ax
        int     $0x10

	movw	$0x1,%cx

	movw	$0x00,%ax
	movw	%ax,LoadAdr+off

	movw	$0x1,%ax # !!!
	movw	%ax,Sektor+off

	movw	$0x3050,%ax
	pushw	%ax
	popw	%ES

loop1:  # Загрузка посекторно
        pushw   %cx

        movw    LoadAdr+off,%bx
        movb    $Drive,%dl
	movw	Sektor+off,%ax
        call    SecRead
        jc      l_err

	movw	$0xe00+'R',%ax
        int     $0x10

	movw	Sektor+off,%ax
	inc	%ax
	movw	%ax,Sektor+off

	movw	LoadAdr+off,%ax
	add	$0x200,%ax
	movw	%ax,LoadAdr+off


        popw    %cx
        loop    loop1

	movw	$0xe00+'S',%ax
        int     $0x10

	# EXEC!
	
        movw    $0x3050,%ax
        movw    %ax,%DS
        cli
        movw    %ax,%SS
        movw    $0xfffe,%SP
        sti

        .byte      0xea    # JMP 3050:0000
        .word      0x0000,0x3050
l_err:
	movw	$0xe00+'E',%ax
        int     $0x10
l_stop_err:	jmp	l_stop_err

;        call    ohw
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

Sektor:	.word
LoadAdr:	.word
Konec:	.word	0xDEAD

