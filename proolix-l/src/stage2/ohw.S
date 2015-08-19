    .code16
    .text
    
    
    .global ohw
    .global ohb
    .global ohd

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
        ja      l_1    # %al > 9
        # %al <= 9
        addb     $0x30,%al	# addb '0',%al
        jmp     l_out

l_1:   addb     $0x37,%al #  addb    'A'-10,%al 
l_out: movb     $0xe,%ah
        int     $0x10

        popw     %ax

        ret
# ohb1    endp
