# Proolix-l first assembler test utility

    .code16gcc

    .include "macros.S"

    .text

    .global	_start

_start:

#	jmp	l_continue
#signature:	.word	0xDDDD
#arguments_offset:	.word	0
#arguments_seg:		.word	0
#l_continue:

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


#    xorb	%ah,%ah
#    int		$0x91 # test function

    int		$0x90	# quit

#    xorb	%ah,%ah
#    int		$0x16	# getchar

#    movb	$1,%ah
#    int		$0x91	#getchar

    int		$0x90

    print str1
    print str1

    print str2

    movb	$1,%ah
    int		$0x91

    print str3
    call	ohw

    print str4

    movb	$'Z',%al
    movb	$2,%ah
    int	$0x91

    int		$0x90

	.include "sayr.S"
	.include "ohw.S"
	.include "saycsip.S"

str1:	.ascii	" First assembler utility for OS Proolix======================================================================="
	.byte	13,10,0
str2:	.ascii	"Press any key "
	.byte	13,10,0
str3:	.ascii	"Register AX: "
	.byte	0
str4:	.asciz	"output char="
