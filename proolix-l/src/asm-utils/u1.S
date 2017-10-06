# Proolix-l first assembler utility

    .code16

    .include "macros.S"

    .global	_start

_start:

    putch $'1'
    putch $'2'
    putch $'3'

    print str1

#    call	print_registers

    # intercept of int 93h
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x93*4+2,%si
    movw	%ax,%ES:(%si)
    lea		l_return,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES

    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0

    movw	$0x248,%si
    ljmp	*%ES:(%si)

l_return:

    putch $'!'

#    call	print_registers

#    int		$0x21

    int		$0x90

	.include "sayr.S"
	.include "ohw.S"
	.include "saycsip.S"

str1:	.asciz	" First assembler utility for OS Proolix. v.2.0 "
