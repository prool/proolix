/* prefix by prool */
/* rtl - runtime library for proolix fortran utilities */

    .code16gcc

    .include "macros.S"

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

/*    putch		$'F' */

    call	MAIN__
    int		$0x90

/*
    call	main
    putch		$'f'
    int		$0x90
*/

_gfortran_st_write:
_gfortran_set_args:
_gfortran_set_options:
/*	putch		$'s' */
	ret

_gfortran_transfer_character_write:
	pushw	%bp
	pushw	%bx

	movw	%sp, %bp

	pushw	%si
	pushw	%cx
	pushw	%ax

/*
	movw	8(%bp), %ax
	call	ohw
	putch	$' '
	movw	12(%bp), %ax
	call	ohw
	putch	$' '
	movw	16(%bp), %ax
	call	ohw
	putch	$':'
*/

	movw	12(%bp),%si
	movw	16(%bp),%cx

prool_l1:
	putch	(%si)
	incw	%si
	loop	prool_l1

/*	putch	$']' */

	popw	%ax
	popw	%cx
	popw	%si

	popw	%bx
	popw	%bp
	ret

_gfortran_st_write_done:
	newline
	ret
/*
ohq: # Вывод сверхдлинного слова (64 разрядного) %rax в hex виде
	# reg saved
	push	%rax

	pop	%rax
	ret
*/

ohd: # Вывод длинного слова (%eax, то есть 8 байт) в hex виде
	# Регистры сохранены
	pushl	%eax
	pushl	%ecx
	
	pushl	%eax
	movb	$16,%cl
	shr	%cl,%eax
	call ohw
	popl	%eax
	call ohw
	popl	%ecx
	popl	%eax
	ret
    
ohw: #     proc

#       Вывод слова в HEX-виде. Вход: слово в %ax.
#       Все регистры сохраняются.
#       Вызывает подпрограмму ohb

        pushw    %ax      # Сохр. ради %al.
        movb     %ah,%al
        call    ohb
        popw     %ax              # Восст. %al.
        call    ohb
        ret
# ohw     endp
ohb: #     proc

# Procedure output hex byte Ver 0.1.1 6 Dec 93 via BIOS
# Input: %al - byte
# All regs. reserved ;)

# Not worked in graph mode. bl - bg color ???

        pushw    %ax
        pushw    %cx
        pushw    %dx

        movb     %al,%dl
        movb     $4,%cl
        shrb     %cl,%al
        call    ohb1

        movb    %dl,%al
        andb    $0xf,%al
        call    ohb1

        popw     %dx
        popw     %cx
        popw     %ax

        ret

# ohb     endp

ohb1: #    proc    # Regs not saved !!!
        pushw    %ax

        cmpb     $9,%al
        ja      ohw_l_1    # %al > 9
        # %al <= 9
        addb     $0x30,%al	# addb '0',%al
        jmp     ohw_l_out

ohw_l_1:   addb     $0x37,%al #  addb    'A'-10,%al 
ohw_l_out: movb     $0xe,%ah
        int     $0x10

        popw     %ax

        ret
# ohb1    endp
/* end of prool prefix */
