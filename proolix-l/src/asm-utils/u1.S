# Proolix-l first assembler utility

    .code16

    .include "macros.S"

    .global	_start

_start:

    putch $'1'
    putch $'2'
    putch $'3'

    print str1

l_loop:	jmp	l_loop

	.include "sayr.S"

str1:	.asciz	" First assembler utility for OS Proolix "
