/* prefix by prool */
/* rtl - runtime library for proolix c-utilities */

    .code16gcc

/*    .include "macros.S" */

    .text

_start:
	.globl	_start

		jmp	l_continue
signature:		.word	0xDDDD
argument_offset:	.word	0
argument_seg:		.word	0
l_continue:

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

    call	main

    int		$0x90

/* end of prool prefix */
