// system call getchar

    .code16gcc
    .text

	.globl	getchar
getchar:
	movb	$1,%ah	# fn 1 - getchar
	int	$0x91
	ret
