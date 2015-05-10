        .code16
	.include "macros.S"
	
	
	.text
.global _start
_start:

    print	str1
    call	print_registers
    newline

    int	$0x20

str1:	.asciz	"CMD1 run! "
