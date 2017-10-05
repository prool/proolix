# Proolix-l first assembler utility

    .code16

    .include "macros.S"

    .global	_start

_start:

    putch $'1'
    putch $'2'
    putch $'3'

    print str1

    call	print_registers

    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0

    movw	$0x248,%si
    lcall	*%ES:(%si)

    putch $'!'

#    call	print_registers

#    int		$0x21

    int		$0x90

	.include "sayr.S"
	.include "ohw.S"
	.include "saycsip.S"

str1:	.asciz	" First assembler utility for OS Proolix. v.2.0 "
