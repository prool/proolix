    .macro	print str
        pushw	%si
        movw	$\str,%si
        call	sayr_proc
        popw	%si
    .endm
    
    .macro	putch chr
          pushw   %ax
	  
	  movb $0x0e,%ah
	  movb \chr,%al
	  
	  int  $0x10
          popw    %ax
    .endm

.macro newline
    putch $13 ; putch $10
.endm
