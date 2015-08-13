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
    

    int	$0x20

	.include "ohw.S"
	.include "sayr.S"
	.include "saycsip.S"


str1:	.asciz	"cmd2com command run. compile under cygwin "
