    .code16gcc
    .text

	.globl	peek
peek:
	pushw	%bp
	pushw	%bx

	movw	%sp, %bp
	movw	8(%bp), %bx
	movw	(%bx),%ax

	popw	%bx
	popw	%bp
	ret

	.globl	peek_far_b
peek_far_b:
	pushw	%bp
	pushw	%bx
	pushw	%ES

	movw	%sp, %bp
	movw	10(%bp), %ES
	movw	12(%bp), %bx
	movb	%ES:(%bx),%al
	xorb	%ah,%ah

	popw	%ES
	popw	%bx
	popw	%bp
	ret
