	.code16
	.include	"macros.S"
	.text
	.global print_registers
print_registers: # proc    ; REGS SAVED
        pushw %ax

	newline

        print    s_ax
        call    ohw

        print    s_cs
        pushw   %CS
        popw    %ax
        call    ohw

	print	s_ip
	call next
next:
	popw	%ax
	call	ohw

        print    s_ss
        movw    %SS,%ax
        call    ohw

        print    s_sp
        movw     %SP,%ax
        call    ohw

	print	s_bp
	movw	%BP,%ax
	call	ohw

        print    s_ds
        movw     %DS,%ax
        call    ohw

        print    s_es
        movw     %ES,%ax
        call    ohw

        print    s_bx
        movw     %bx,%ax
        call    ohw

        print    s_cx
        movw     %cx,%ax
        call    ohw

        print    s_dx
        movw     %dx,%ax
        call    ohw

        print    s_si
        movw     %si,%ax
        call    ohw

        print    s_di
        movw     %di,%ax
        call    ohw

        popw    %ax
        ret
s_cs:    .asciz      " CS="
s_ss:    .asciz      " SS="
s_sp:    .asciz      " SP="
s_ds:    .asciz      " DS="
s_es:    .asciz      " ES="
s_ax:    .asciz      "AX="
s_bx:    .asciz      " BX="
s_cx:    .asciz      " CX="
s_dx:    .asciz      " DX="
s_bp:    .asciz      " BP="
s_si:    .asciz      " SI="
s_di:    .asciz      " DI="
s_ip:    .asciz      " IP="


	.global print_reg32
print_reg32: # proc    ; REGS SAVED
        push %eax

#	mov	$0xDEADBEEF,%eax
        print    s_eax
        call    ohd

        print    s_esp
        mov     %ESP,%eax
        call    ohd

	print	s_ebp
	mov	%EBP,%eax
	call	ohd

        print    s_ebx
        mov     %ebx,%eax
        call    ohd

        print    s_ecx
        mov     %ecx,%eax
        call    ohd

        print    s_edx
        mov     %edx,%eax
        call    ohd

        print    s_esi
        mov     %esi,%eax
        call    ohd

        print    s_edi
        mov     %edi,%eax
        call    ohd

        pop    %eax
        ret
s_esp:    .asciz      " ESP="
s_ees:    .asciz      " EES="
s_eax:    .asciz      "EAX="
s_ebx:    .asciz      " EBX="
s_ecx:    .asciz      " ECX="
s_edx:    .asciz      " EDX="
s_ebp:    .asciz      " EBP="
s_esi:    .asciz      " ESI="
s_edi:    .asciz      " EDI="
s_eip:    .asciz      " EIP="
