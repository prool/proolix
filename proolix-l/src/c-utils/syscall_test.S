    .code16gcc
    .text

	.globl	syscall_test
syscall_test:
	    movb	$0,%ah	# fn 0 - test
	    int		$0x91
	ret
