    .code16gcc
    .text

    .global	open
open:
	pushw	%bp

	movw	%sp, %bp
	movw	6(%bp), %bx	# ES:bx - filename

	pushw	%CS
	popw	%ES

	movb	10(%bp), %al	# filemode

	movb	$0x20,%ah	# fn open
	int	$0x91

#	call	sayr_proc

	popw	%bp
	ret
