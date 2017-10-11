# Proolix-l first assembler test utility

    .code16gcc

    .include "macros.S"

    .global	_start

_start:

    print str1

    movb	$0,%ah	# fn 0 - test
    int		$0x91

    print str2

    movb	$1,%ah
    int		$0x91

    print str3
    call	ohw

    print str4

    movb	$'Z',%al
    movb	$2,%ah
    int	$0x91

    int		$0x90

	.include "sayr.S"
	.include "ohw.S"
	.include "saycsip.S"

str1:	.ascii	" First assembler utility for OS Proolix"
	.byte	13,10,0
str2:	.ascii	"Press any key "
	.byte	13,10,0
str3:	.ascii	"Register AX: "
	.byte	0
str4:	.asciz	"output char="
