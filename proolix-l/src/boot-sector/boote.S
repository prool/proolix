# Proolix-l bedug boot sector

# russian text in UTF-8

# by Serge Pustovoitoff, 1993-2015

# При отладке бута учитывались многочисленные полезные замечания, которые
# сделали Алекс Семеняка и Юрий Белотицкий

# Смещение, связанное с тем, что во время компиляции
# адресом начала является 0, а во время
# выполнения 0x7c00

#include	"macros.S"

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

# /* 0:0 - 0:200 - таблица прерываний */
# Стек: c 30:F6 (0:3F6) растет к 0 (0:200-0:400 - стек)
# ROM BIOS area 0:400-0:500

        pushw   %CS
        popw    %ES

        pushw   %CS
        popw    %DS

	putch	$'='
	print	s_hello+off

	pushw	%ES
	xorw	%di,%di
	movw	%di,%ES
	movb	$8,%ah
	movb	$0x80,%dl
	int	$0x13
	jc	l_err
	call	print_registers
	popw	%ES
ll:	jmp	ll	
s_hello:	.asciz	" Proolix debug bootsector "

l_err:
        putch   $'e'
        putch	$'r'
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

.include	"saycsip.S"

Buff:
