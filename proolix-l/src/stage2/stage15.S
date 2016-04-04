# Proolix-l stage1.5 for HDD (GRUB-like)
# by Prool
# www.prool.kharkov.org
# russian comments in UTF-8 codetable
        .code16
	.include "macros.S"
STRLEN	=	80
Drive	=	0	# drive A: (FDD 0, /dev/fd0) 
KernelSeg	=	0x60
	.text
.global _start
_start:
    jmp		l_1
    .byte 0xDD
    .byte 0xDD
str1:	.byte		13,10
	.ascii		"Proolix-l HDD/stage1.5 2blocks"
	.byte 		13,10,0
l_1:
    putch	$'#'
    jmp		l_2
    . = 512
l_2:
    print	str1
l_stop: jmp l_stop
//	.include "ohw.S"
	.include "sayr.S" 
//	.include "saycsip.S"
