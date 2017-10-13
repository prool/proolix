    .code16gcc
    .text

	.globl	putch
putch:
	pushw	%bp

	movw	%sp, %bp
	movb	6(%bp), %al
	movb	$2,%ah	# fn 2 - putchar
	int	$0x91

	popw	%bp
	ret
