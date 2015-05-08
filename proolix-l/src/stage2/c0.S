# main program (RTL) for run C-language kernel (stage2)
# not work!
        .code16

	    .text
.global _start
_start:

	call	c_kernel
