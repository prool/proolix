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
	mov	%bx,%SS
	movw	$0xFFFC,%SP
	sti

// for debug
	  movb $0x0e,%ah
	  movb $'1',%al
	  int  $0x10	# putch

    print str1

    movb	$0,%ah	# fn 0 - test
    int		$0x91

    print str3
    call	ohw

    print str4

    movb	$'Z',%al
    movb	$2,%ah
    int	$0x91

    call	main

    int		$0x90

	.include "sayr.S"
	.include "ohw.S"
#	.include "saycsip.S"

str1:	.ascii	" First C utility for OS Proolix"
	.byte	13,10,0
str2:	.ascii	"Press any key "
	.byte	13,10,0
str3:	.ascii	"Register AX: "
	.byte	0
str4:	.asciz	"output char="

/* end of prool prefix */


