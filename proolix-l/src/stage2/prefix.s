# assembler prefix file for c-translated text

	.text
	.code16gcc
	jmp	_main

_putch2:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	movl	8(%ebp), %eax
	movb	%al, -4(%ebp)
	movb $9,%ah
	movzbl	-4(%ebp), %edx
	movb %dl,%al
	xorb %bh,%bh
	movb	$3,%bl
	movw	$1,%cx
	int $0x10
	leave
	ret

_putch_color:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$12, %esp
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	movb	%dl, -4(%ebp)
	movb	%al, -8(%ebp)
	movsbl	-4(%ebp), %eax
#	movb	%al,%al # par 1
	movsbl	-8(%ebp), %ecx
	movb	%cl,%bl # par 2

	movw	$1,%cx
	movb	$9,%ah
	xorb	%bh,%bh
	int	$0x10
	leave
	ret

_setpos_:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$12, %esp
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
	movb	%dl, -4(%ebp)
	movb	%al, -8(%ebp)
	movsbl	-4(%ebp), %eax
	movb	%al,%dh # ROW
	movsbl	-8(%ebp), %eax
	movb	%al,%dl # COL
	movb	$2,%ah
	xorb	%bh,%bh
	int	$0x10
	leave
	ret
/*
_getpos:
	pushl	%ebp
	movl	%esp, %ebp

	movb	$3,%ah
	xorb	%bh,%bh
	int	$0x10

	xor	%ebx,%ebx	# ebx:=0
	movb	%dh,%bl

	movl	8(%ebp), %eax
	movl	%ebx, (%eax)	# param1

	xor	%ebx,%ebx	# ebx:=0
	movb	%dl,%bl

	movl	12(%ebp), %eax
	movl	%ebx, (%eax)	# param2
	popl	%ebp
	ret
*/
