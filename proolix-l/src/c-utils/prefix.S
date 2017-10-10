/* prefix by prool */

    .code16gcc

    .include "macros.S"

    .text

_start:
	.globl	_start

	push	%cs
	pop	%ds

	push	%cs
	pop	%es

	push	%cs
	pop	%bx

	cli
	movw	%bx,%SS
	movw	$0xFFFC,%SP
	sti

#	call	print_registers

// for debug
	  movb $0x0e,%ah
	  movb $'1',%al
	  int  $0x10	# putch

#    call	syscall_test
#    call	subroutine

#    xorb	%ah,%ah
#    int		$0x91

    call	main

    int		$0x90

subroutine:
    ret

    .globl	syscall_test
syscall_test:
// for debug
	  movb $0x0e,%ah
	  movb $'3',%al
	  int  $0x10	# putch
	    movb	$0,%ah	# fn 0 - test
//$$$	    int		$0x91
// for debug
	  movb $0x0e,%ah
	  movb $'4',%al
	  int  $0x10	# putch
	ret

    .globl	stop
stop:
// for debug
	  movb $0x0e,%ah
	  movb $'S',%al
	  int  $0x10	# putch
	int	$0x90

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


