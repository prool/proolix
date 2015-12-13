#saycsip for bootsectors
	.code16
	.text
	.global print_registers
print_registers: # proc    ; REGS SAVED
        pushw %ax

	newline

        print    s_ax+off
        call    ohw

        print    s_cs+off
        pushw   %CS
        popw    %ax
        call    ohw

	print	s_ip+off
	call next
next:
	popw	%ax
	call	ohw

        print    s_ss+off
        movw    %SS,%ax
        call    ohw

        print    s_sp+off
        movw     %SP,%ax
        call    ohw

	print	s_bp+off
	movw	%BP,%ax
	call	ohw

        print    s_ds+off
        movw     %DS,%ax
        call    ohw

        print    s_es+off
        movw     %ES,%ax
        call    ohw

        print    s_bx+off
        movw     %bx,%ax
        call    ohw

        print    s_cx+off
        movw     %cx,%ax
        call    ohw

        print    s_dx+off
        movw     %dx,%ax
        call    ohw

        print    s_si+off
        movw     %si,%ax
        call    ohw

        print    s_di+off
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
