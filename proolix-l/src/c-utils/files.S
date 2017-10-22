// system calls for work with files: open() readc() writec() close()

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

	.global	readc
readc:
	pushw	%bp
	movw	%sp,%bp
	# 6(%bp) file descriptor
	# 10(%bp) address for read

	# read file
		# input: al - file descriptor
		# output:
		#	al - readed character
		#	ah - error code

	movb	$0x21,%ah
	movb	6(%bp),%al
	int	$0x91

	movw	10(%bp),%bx
	movb	%al,(%bx)

	movb	%ah,%al
	xorb	%ah,%ah

	popw	%bp
	ret

	.global	writec
	# write file
		# input: al - file descriptor
		#	dl - writed character
		# output:
		#	ax - error code
writec:
	pushw	%bp
	movw	%sp,%bp

	movb	$0x22,%ah
	movb	6(%bp),%al
	movb	10(%bp),%dl
	int	$0x91

	popw	%bp
	ret

	.global	close
close:
	pushw	%bp
	movw	%sp,%bp

	movb	6(%bp),%al
	movb	$0x23,%ah
	int	$0x91

	popw	%bp
	ret
