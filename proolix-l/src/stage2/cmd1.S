        .code16
	.include "macros.S"
	
	
	.text
.global _start
_start:

    print	str1
    call	print_registers
    newline

l_stop:	jmp l_stop

str1:	.asciz	"CMD1 run! "
