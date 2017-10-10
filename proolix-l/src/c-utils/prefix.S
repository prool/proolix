/* prefix by prool */

    .code16

    .include "macros.S"

    .text

_start:
	.globl	_start

	pushw	%cs
	popw	%ds

	pushw	%cs
	popw	%es

	pushw	%cs
	popw	%bx

	cli
	movw	%bx,%SS
	movw	$0xFFFC,%SP
	sti

#	call	print_registers

#    putch	$'1'

#    call	syscall_test
#    call	subroutine

#    xorb	%ah,%ah
#    int		$0x91

    call	main

#    putch	$'m'

    int		$0x90

subroutine:
    ret

    .globl	syscall_test
syscall_test:
#	putch	$'3'
	    movb	$0,%ah	# fn 0 - test
	    int		$0x91
#	putch	$'4'
	ret

    .globl	stop
stop:
#	putch	$'S'
	int	$0x90

    .globl	getchar
getchar:
	movb	$1,%ah	# fn 1 - getchar
	int	$0x91
	call	ohw
	ret

putch:

	movw	%sp, %bp
	movb	2(%bp), %al
	movb	$2,%ah	# fn 2 - putchar
	int	$0x91
	ret

	.include "sayr.S"
	.include "ohw.S"
	.include "saycsip.S"

str1:	.ascii	" First C utility for OS Proolix"
	.byte	13,10,0
str2:	.ascii	"Press any key "
	.byte	13,10,0
str3:	.ascii	"Register AX: "
	.byte	0
str4:	.asciz	"output char="

/* end of prool prefix */
