        .code16
	.include "macros.S"
	
	
	.text
.global _start
_start:

/*
    movw	$0x0EA0,%ax
    int		$0x10
    xorb	%ah,%ah
    int		$0x16
    */
    
    print	str1
    call	print_registers
    newline
    
	pushl	-4(%ecx)

    int	$0x20

str1:	.asciz	"CMD1 run. And pushl -4(%ecx) command"
