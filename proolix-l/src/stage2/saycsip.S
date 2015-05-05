	.code16
	.include "macros.S"
	.text
	.global print_registers
print_registers: # proc    ; REGS SAVED
        pushw %ax

        print    s0
        pushw   %CS
        popw    %ax
        call    ohw

        print    s_ss
        movw    %SS,%ax
        call    ohw

        print    s_sp
        movw     %SP,%ax
        call    ohw

        print    s_ds
        movw     %DS,%ax
        call    ohw

        print    s_es
        movw     %ES,%ax
        call    ohw

        popw    %ax
        ret
s0:     .byte      13,10
	.asciz	"CS="
s_ss:    .asciz      " SS="
s_sp:    .asciz      " SP="
s_ds:    .asciz      " DS="
s_es:    .asciz      " ES="
