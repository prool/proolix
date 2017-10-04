# Proolix-l first assembler utility

    .code16

    .include "macros.S"

    .global	_start

_start:

    putch $'1'
    putch $'2'
    putch $'3'

    print str1

    int		$0x90

	.include "sayr.S"

str1:	.asciz	" First assembler utility for OS Proolix. v.2.0 "
