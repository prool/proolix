# assembler prefix file for c-translated text

# some comments from Ralf Brown Interrupt list http://www.ctyme.com/intr/int.htm
# Interrupt list rulez! prool

	.include "macros.S"

	.text
	.code16gcc
_start:
	.globl	_start

	jmp	obhod
# human readable magick string
	.ascii	" Kernel "

arguments:
	.fill	100,1,0
g_filename:
	.fill	17,1,0
obhod:
	push	%cs
	pop	%ds

	push	%cs
	pop	%es

	push	%cs
	pop	%bx

	cli
	mov	%bx,%SS
	movw	$0xFFFC,%SP
	sti

	movw	%ax,boot_drive

// for debug
	  movb $0x0e,%ah
	  movb $'_',%al
	  int  $0x10	# putch

/*
    # intercept of int 15h	for debug!
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x15*4+2,%si
    movw	%ax,%ES:(%si)
    lea		int_15,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES
*/

/*
    # intercept of int 4h	CPU-generated - INTO DETECTED OVERFLOW
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x4*4+2,%si
    movw	%ax,%ES:(%si)
    lea		interrupt_4,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES
*/

    # intercept of int 90h	Proolix: terminate proolix program (similar to int 20h in MSDOS)
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x90*4+2,%si
    movw	%ax,%ES:(%si)
    lea		interrupt_90,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES

    # intercept of int 91h	Proolix: various functions (similar to int 21h in MSDOS)
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x91*4+2,%si
    movw	%ax,%ES:(%si)
    lea		interrupt_91,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES
/*
    # intercept of int 92h	Proolix: dirty functions (obsolete, experimental)
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x92*4+2,%si
    movw	%ax,%ES:(%si)
    lea		interrupt_92,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES
*/
    # intercept of int 21h (MSDOS interrupt)

    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
#    movw	$0xDEAD,%ax
    pushw	%CS
    popw	%ax
    movw	$0x86,%si	# 21*4+2
    movw	%ax,%ES:(%si)
#    movw	$0xBEEF,%ax
    lea		int21p,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES

    # intercept of int 20h (DOS 1+ - TERMINATE PROGRAM)
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x82,%si	# 20*4+2
    movw	%ax,%ES:(%si)
    lea		int20p,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES

/*
    # intercept of int 24h DOS 1+ - CRITICAL ERROR HANDLER
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x24*4+2,%si
    movw	%ax,%ES:(%si)
    lea		int24p,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES


    # intercept of int 0h	Zenith - ROM DEBUGGER (Interrupt List by Ralf Brown)
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x0*4+2,%si
    movw	%ax,%ES:(%si)
    lea		interrupt_0,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES

    # intercept of int 1h	CPU-generated - SINGLE STEP
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x1*4+2,%si
    movw	%ax,%ES:(%si)
    lea		interrupt_1,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES

    # intercept of int 2h	External hardware - NON-MASKABLE INTERRUPT
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x2*4+2,%si
    movw	%ax,%ES:(%si)
    lea		interrupt_2,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES

    # intercept of int 3h	CPU-generated - BREAKPOINT
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x3*4+2,%si
    movw	%ax,%ES:(%si)
    lea		interrupt_3,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES
*/
    # intercept of int 5h - print screen
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x5*4+2,%si
    movw	%ax,%ES:(%si)
    lea		interrupt_5,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES
/*
    # intercept of int 6h	CPU-generated (80186+) - INVALID OPCODE
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x6*4+2,%si
    movw	%ax,%ES:(%si)
    lea		interrupt_6,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES

    # intercept of int 7h	CPU-generated (80286+) - PROCESSOR EXTENSION NOT AVAILABLE
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x7*4+2,%si
    movw	%ax,%ES:(%si)
    lea		interrupt_7,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES

    # intercept of int ah	IRQ2 - LPT2 (PC), VERTICAL RETRACE INTERRUPT (EGA,VGA)
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0xa*4+2,%si
    movw	%ax,%ES:(%si)
    lea		interrupt_a,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES

    # intercept of int bh	IRQ3 - SERIAL COMMUNICATIONS (COM2)	CPU-generated (80286+) - SEGMENT NOT PRESENT
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0xb*4+2,%si
    movw	%ax,%ES:(%si)
    lea		interrupt_b,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES

    # intercept of int ch	RQ4 - SERIAL COMMUNICATIONS (COM1)	CPU-generated (80286+) - STACK FAULT
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0xc*4+2,%si
    movw	%ax,%ES:(%si)
    lea		interrupt_c,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES

    # intercept of int dh	IRQ5 - FIXED DISK (PC,XT), LPT2 (AT), reserved (PS/2)
    #  CPU-generated (80286+) - GENERAL PROTECTION VIOLATION
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0xd*4+2,%si
    movw	%ax,%ES:(%si)
    lea		interrupt_d,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES
*/
    
// for debug
	  movb $0x0e,%ah
	  movb $'+',%al
	  int  $0x10	# putch

	jmp	main
/*
int_15:	
	putch	$'#'
	  .code16
	  iret
	  .code16gcc
*/
run:
        # EXEC!

	pushw	%CS
	popw	%bx

        movw    $0x4050,%ax	# segment
        movw    %ax,%DS
	movw	%ax,%ES
	movw	$arguments,%ES:(6)
	movw	%bx,%ES:(8)
        cli
        movw    %ax,%SS
        movw    $0xfff0,%SP # !!! fffe? fffd??
        sti

        .byte      0xea    # JMP stage2_seg:0000
        .word      0x0000,0x4050

run_msdos:
        # EXEC!

        movw    $0x4050,%ax	# segment
        movw    %ax,%DS
	movw	%ax,%ES
        cli
        movw    %ax,%SS
        movw    $0xfff0,%SP
        sti

	/* формирование PSP */
	movw	$0x20CD,%ax
	movw	%ax,%ES:(0)

	movw	$0x5050,%ax
	movw	%ax,%ES:(2)
/*
	movb	$0,%al
	movb	%al,%ES:(80)

	movb	$0x0D,%al
	movb	%al,%ES:(81)
*/

	movb	$3,%al
	movb	%al,%ES:(80)

	movb	$'/',%al
	movb	%al,%ES:(81)

	movb	$'?',%al
	movb	%al,%ES:(82)

	movb	$0x0D,%al
	movb	%al,%ES:(83)

        .byte      0xea    # JMP stage2_seg:0100
        .word      0x0100,0x4050
/*
interrupt_92:

	pushw	%DS

	  movb $0x0e,%ah
	  movb $'F',%al
	  int  $0x10	# putch

	  pushw		%CS
	  popw		%DS

#  call	print_registers

	movw	$s_txt92, %si
	call	sayr_proc

    	xorw	%ax,%ax
    	movw	%ax,%ES # ES:=0

    	popw	%DS

    	movw	$0x24C,%si
    	ljmp	*%ES:(%si)

s_txt92:	.asciz " int 92 say "
s_txt91:	.asciz " int 91 !!!111 "
*/

interrupt_91:	# Proolix functions (similar to int 21h in MSDOS)

	  pushw	%DS # save DS

	  pushw %CS
	  popw	%DS # DS:=CS

/*
	  cmpb	$0,%ah		# function 0 - test
	  je	l_91_0
*/
	  cmpb	$1,%ah		# function 1 - input character from console
	  je	l_91_1
	  cmpb	$2,%ah		# function 2 - output character from %al
	  je	l_91_2

	  			# function 3 - test of input character

	  			# function 0x10 - read phys. sector addressed CHS (?)

	  			# function 0x11 - write phys. sector addressed CHS (?)

	  			# function 0x12 - read absolute phys. sector numer N (?)

	  			# function 0x13 - write absolute phys. sector numer N (?)

	cmpb	$0x20,%ah	# function 0x20 - open file
	je	l_91_20
	  			# function 0x21 - read file
	cmpb	$0x21,%ah
	je	l_91_21
	  			# function 0x22 - write file
	cmpb	$0x22,%ah
	je	l_91_22
	  			# function 0x23 - close file
	cmpb	$0x23,%ah
	je	l_91_23
	  			# function 0x24 - delete file

				# function 0x25 - append file

	  			# function 0x30 - exec file

	  jmp	l_91_unknown_fn
/*
l_91_0:		call	test91
#putch	$'@'
		jmp	l_91_exit
*/
l_91_1:
	xorb	%ah,%ah
	int	$0x16
	xorb	%ah,%ah
	jmp	l_91_exit

l_91_2:
	#xorb	%bh,%bh # ?????
	pushl	%eax
	call	putch
	incw	%sp
	incw	%sp
	incw	%sp
	incw	%sp

	jmp	l_91_exit

l_91_20:	# open file
		# input: es:bx - filename, al - filemode (f.e. O_READ)
		# output: ax - descriptor (-1 - error)

/*
	pushw	%ax
	movw	%es,%ax
	call	ohw
	movw	%bx,%ax
	call	ohw
	popw	%ax
	call	ohw
*/

	movw	$g_filename,%si
	movw	$16,%cx
l_91_20_1:
	movb	%ES:(%bx),%dh
	movb	%dh,(%si)
	testb	%dh,%dh
	jz	l_91_20_2
	incw	%bx
	incw	%si

	loop	l_91_20_1
l_91_20_2:
	pushw	$0
	xorb	%ah,%ah
	pushw	%ax
	pushl	$g_filename
	call	open_
	addw	$8,%sp

	jmp	l_91_exit

l_91_21:	# read file
		# input: al - file descriptor
		# output:
		#	al - readed character
		#	ah - error code
		leal	char_buffer,%ebx
		pushl	%ebx
		pushw	$0
		xorb	%ah,%ah
		pushw	%ax
		call	readc
		addw	$8,%sp
		movb	%al,%ah
		movb	char_buffer,%al
	jmp	l_91_exit
char_buffer:	.byte	0

l_91_22:	# write file
		# input: al - file descriptor
		#	dl - writed character
		# output:
		#	ax - error code
		pushl	%edx
		pushw	$0
		xorb	%ah,%ah
		pushw	%ax
		call	writec
		addw	$8,%sp
	jmp	l_91_exit

l_91_23:	# close file
		# input al - file descriptor
		# output ax - error code
		xorb	%bh,%bh
		pushl	%eax
		call	close_
		addw	$4,%sp
		jmp	l_91_exit

l_91_unknown_fn:
		call	int91_unknown

l_91_exit:

	  popw	%DS # restore DS

	  .code16
	  iret
	  .code16gcc

	  /* composite IRET ;) */
	  /*
	  incw	%SP
	  incw	%SP
	  incw	%SP
	  incw	%SP
	  incw	%SP
	  incw	%SP

	  ljmp	*%SS:(%bp)
	  */
/*
s_interrupt_90:	.ascii	"exit"
		.byte	13,10,0
*/
interrupt_90:	# 

	pushw	%CS
	popw	%DS	# DS:=CS

	pushw	%CS
	popw	%ax
	movw	%ax,%ES
	cli
	movw	%ax,%SS
	movw	$0xFFFE,%SP
	sti

#	print s_interrupt_90

	jmp	main
/*
interrupt_0:	# 
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_0

	popw	%DS
	iret

s_interrupt_0:	.asciz	" Int 0 "

interrupt_1:	# 
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_1

	popw	%DS
	iret

s_interrupt_1:	.asciz	" Int 1 "

interrupt_2:	# 
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_2

	popw	%DS
	iret

s_interrupt_2:	.asciz	" Int 2 "

interrupt_3:	# 
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_3

	popw	%DS
	iret

s_interrupt_3:	.asciz	" Int 3 "

interrupt_4:	# 
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_4

	popw	%DS
	iret

s_interrupt_4:	.asciz	" Int 4 CPU-generated - INTO DETECTED OVERFLOW"
*/
interrupt_5:	# Print screen. In Proolix: break program
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_5

	popw	%DS

	jmp	interrupt_90
/*
	  .code16
	  iret
	  .code16gcc
*/
s_interrupt_5:	.asciz	" Int 5 - printscreen "
/*
interrupt_6:	# 
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_6

	popw	%DS
	iret

s_interrupt_6:	.asciz	" Int 6 "

interrupt_7:	# 
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_7

	popw	%DS
	iret

s_interrupt_7:	.asciz	" Int 7 "

interrupt_a:	# 
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_a

	popw	%DS
	iret

s_interrupt_a:	.asciz	" Int a "

interrupt_b:	# 
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_b

	popw	%DS
	iret

s_interrupt_b:	.asciz	" Int b "

interrupt_c:	# 
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_c

l_int_c_stop:	jmp l_int_c_stop

	popw	%DS
	iret

s_interrupt_c:	.asciz	" Int C "

interrupt_d:	# 
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_d
l_int_d_stop:	jmp l_int_d_stop
	
	movb	$0x20,%al
	outb	%al,$0x20

	popw	%DS
	iret

s_interrupt_d:	.asciz	" int d GENERAL PROTECTION VIOLATION "
*/

/*
int24p:
    pushw	%CS
    popw	%DS # DS:=CS
	print l_crit
	jmp	l_msdos_exit
l_crit:	.asciz	" DOS CRITICAL ERROR "


int20p:
    pushw	%CS
    popw	%DS # DS:=CS
	jmp	l_msdos_exit
*/

/********************************* MSDOS emulator *****************************/
int20p:
	jmp	l_21_4c

int21p:
	movw	%SP,%BP

	cmpb	$0x0,%ah	# DOS 1+ - TERMINATE PROGRAM
	jz	l_21_0

	cmpb	$0x1,%ah	# DOS 1+ - READ CHARACTER FROM STANDARD INPUT, WITH ECHO
	jz	l_21_1

	cmpb	$0x2,%ah	# DOS 1+ - WRITE CHARACTER TO STANDARD OUTPUT
	jz	l_21_2

	cmpb	$0x6,%ah	# DOS 1+ - DIRECT CONSOLE OUTPUT
	jz	l_21_2		# prool: maybe same as ah=2 !

	cmpb	$0x7,%ah	# DIRECT CHARACTER INPUT, WITHOUT ECHO
	jz	l_21_1		# prool: maybe same as ah=1		

	cmpb	$0x8,%ah	# CHARACTER INPUT WITHOUT ECHO
	jz	l_21_1		# prool: maybe same as ah=1		

	cmpb	$0x9,%ah	# DOS 1+ - WRITE STRING TO STANDARD OUTPUT
	jz	l_21_9

	cmpb	$0xb,%ah	# GET STDIN STATUS
	jz	l_21_b

	cmpb	$0x1a,%ah	# DOS 1+ - SET DISK TRANSFER AREA ADDRESS
	jz	l_21_1a

	cmpb	$0x25,%ah	# DOS 1+ - SET INTERRUPT VECTOR
	jz	l_21_25

	cmpb	$0x29,%ah	# DOS 1+ - PARSE FILENAME INTO FCB
	jz	l_21_29

	cmpb	$0x2a,%ah	# DOS 1+ - GET SYSTEM DATE
	jz	l_21_2a

	cmpb	$0x30,%ah	# DOS 2+ - GET DOS VERSION
	jz	l_21_30

	cmpb	$0x34,%ah	# DOS 2+ - GET ADDRESS OF INDOS FLAG
	jz	l_21_34

	cmpb	$0x35,%ah	# DOS 2+ - GET INTERRUPT VECTOR
	jz	l_21_35

	cmpb	$0x37,%ah	# DOS 2+ - SWITCHAR - GET SWITCH CHARACTER
	jz	l_21_37

	cmpb	$0x3d,%ah	# DOS 2+ - OPEN - OPEN EXISTING FILE
	jz	l_21_3d

	cmpb	$0x3f,%ah	# DOS 2+ - READ - READ FROM FILE OR DEVICE
	jz	l_21_3f

	cmpb	$0x40,%ah	# DOS 2+ - WRITE - WRITE TO FILE OR DEVICE
	jz	l_21_40

	cmpb	$0x44,%ah	# DOS 2+ - IOCTL - GET DEVICE INFORMATION
	jz	l_21_44

	cmpb	$0x48,%ah	# DOS 2+ - ALLOCATE MEMORY
	jz	l_21_48

	cmpb	$0x49,%ah	# DOS 2+ - FREE MEMORY
	jz	l_21_49

	cmpb	$0x4a,%ah	# DOS 2+ - RESIZE MEMORY BLOCK
	jz	l_21_4a

	cmpb	$0x4b,%ah	# DOS 2+ - EXEC - LOAD AND/OR EXECUTE PROGRAM
	jz	l_21_4b

	cmpb	$0x4c,%ah	# DOS 2+ - EXIT - TERMINATE WITH RETURN CODE
	jz	l_21_4c

	cmpb	$0x50,%ah	# DOS 2+ internal - SET CURRENT PROCESS ID (SET PSP ADDRESS)
	jz	l_21_50

	cmpb	$0x52,%ah	# DOS 2.11+ - GET OR SET MEMORY ALLOCATION STRATEGY
	jz	l_21_52

	cmpb	$0x55,%ah	# DOS 2+ internal - CREATE CHILD PSP
	jz	l_21_55

	cmpb	$0x5d,%ah	# DOS 3.1+ internal - SERVER FUNCTION CALL
	jz	l_21_5d

	cmpb	$0x58,%ah	# DOS 2.11+ - GET OR SET MEMORY ALLOCATION STRATEGY
	jz	l_21_58

	cmpb	$0x60,%ah	# DOS 3.0+ - TRUENAME - CANONICALIZE FILENAME OR PATH
	jz	l_21_60

	jmp	l_21_unkn_fn

l_21_0: # DOS 1+ - TERMINATE PROGRAM
	jmp	l_21_4c

l_21_1: # READ CHARACTER FROM STANDARD INPUT, WITH ECHO
	xorb	%ah,%ah
	int	$0x16
	xorb	%ah,%ah
	jmp	l_21_exit

l_21_2:	# DOS 1+ - WRITE CHARACTER TO STANDARD OUTPUT DL = character to write
	movb	$0x0e, %ah
	movb	%dl,%al
	xorw	$1,%bx
	movw	$1,%cx
	int	$0x10
	jmp	l_21_exit

l_21_9:	# AH=9 DOS 1+ - WRITE STRING TO STANDARD OUTPUT
	pushw	%si

	movw	%dx,%si
l_21_9_0:
	movb	%DS:(%si),%al
	cmpb	$'$', %al
	je	l_21_9_exit
	movb $0x0e,%ah
	int  $0x10	# putch

	incw	%si
	jmp	l_21_9_0
l_21_9_exit:
	movb	$0x24,%al
	popw	%si
	
	jmp	l_21_exit

l_21_b: # GET STDIN STATUS Return: AL = status 00h if no character available FFh if character is available
	movb	$1,%ah
	int	$0x10
	jz	l_21_b_symbol_here
	xorb	%al,%al
	jmp	l_21_exit
l_21_b_symbol_here:
	movb	$0xFF,%al
	jmp	l_21_exit

l_21_1a: # DOS 1+ - SET DISK TRANSFER AREA ADDRESS
	jmp	l_21_exit

l_21_25: # DOS 1+ - SET INTERRUPT VECTOR
	pushw	%ES
	pushw	%cx

	xorw	%cx,%cx
	movw	%cx,%ES # ES=0

	xorb	%ah,%ah # ah=0, al=interrupt number; ax=interrupt number
	shlw	$0x2,%ax # ax=ax*4
	# ax - address of interrupt vector (offset)

	movw	%ax,%si
	movw	%dx,%es:(%si) # offset := DX

	incw	%si
	incw	%si

	pushw	%DS
	popw	%ax	# ax:=DS
	movw	%ax,%es:(%si) # segment := DOS DS

	popw	%cx
	popw	%ES
	jmp	l_21_exit

l_21_29:	# DOS 1+ - PARSE FILENAME INTO FCB
#call	sayr_proc
	jmp	l_21_exit

l_21_2a:	# DOS 1+ - GET SYSTEM DATE
# Return: CX = year (1980-2099) DH = month DL = day ---DOS 1.10+--- AL = day of week (00h=Sunday)

	movb	$4,%ah
	int	$0x1a	# GET REAL-TIME CLOCK DATE (AT,XT286,PS)
	# Return: CH = century (BCD) CL = year (BCD) DH = month (BCD) DL = day (BCD)

	movb	%dl,%al
	shrb	$4,%al
	movb	$10,%bl
	mul	%bl
	movb	%dl,%bl
	andb	$0x0F,%bl
	xorb	%bh,%bh
	addw	%bx,%ax
	movw	%ax,day

	movb	%dh,%al
	shrb	$4,%al
	movb	$10,%bl
	mul	%bl
	movb	%dh,%bl
	andb	$0x0F,%bl
	xorb	%bh,%bh
	addw	%bx,%ax
	movw	%ax,month

	movb	%ch,%al
	shrb	$4,%al
	movb	$10,%bl
	mul	%bl
	movb	%ch,%bl
	andb	$0x0F,%bl
	xorb	%bh,%bh
	addw	%bx,%ax
	movw	%ax,century

	movb	%cl,%al
	shrb	$4,%al
	movb	$10,%bl
	mul	%bl
	movb	%cl,%bl
	andb	$0x0F,%bl
	xorb	%bh,%bh
	addw	%bx,%ax
	movw	%ax,year

	movw	century,%ax
	movw	$100,%bx
	mul	%bx
	movw	year,%cx
	addw	%cx,%ax
# Return: CX = year (1980-2099) DH = month DL = day ---DOS 1.10+--- AL = day of week (00h=Sunday)
	movw	%ax,%cx
	movb	month,%dh
	movb	day,%dl
	xorb	%al,%al

	jmp	l_21_exit
day:		.word	0
month:		.word	0
century:	.word	0
year:		.word	0

l_21_30:	# DOS 2+ - GET DOS VERSION
	xorw	%ax,%ax		# DOS 1.x ;-)
	jmp	l_21_exit

l_21_34:	# DOS 2+ - GET ADDRESS OF INDOS FLAG
	jmp	l_21_exit	# prool: ;-)

l_21_35: # DOS 2+ - GET INTERRUPT VECTOR
	pushw	%cx

	xorw	%cx,%cx
	movw	%cx,%ES # ES=0

	xorb	%ah,%ah # ah=0, al=interrupt number; ax=interrupt number
	shlw	$0x2,%ax # ax=ax*4
	# ax - address of interrupt vector (offset)

	movw	%ax,%si
	movw	%es:(%si),%bx # offset -> bx

	incw	%si
	incw	%si

	movw	%es:(%si),%ax
	movw	%ax,%es # segment -> ES

	popw	%cx
	jmp	l_21_exit

l_21_37:	# DOS 2+ - SWITCHAR - GET SWITCH CHARACTER
	xorb	%al,%al
	movb	$'/',%dl
	jmp	l_21_exit

l_21_3d:	# DOS 2+ - OPEN - OPEN EXISTING FILE
	movw	$2,%ax	# err: file not found
	stc
	jmp	l_21_exit

l_21_3f:	# DOS 2+ - READ - READ FROM FILE OR DEVICE
	movw	$5,%ax # err: access denied
	stc
	jmp	l_21_exit

l_21_40: # DOS 2+ - WRITE - WRITE TO FILE OR DEVICE
	movw	$5,%ax # err
	stc
	jmp	l_21_exit

l_21_44: # INT 21 - DOS 2+ - IOCTL - GET DEVICE INFORMATION
	movw	$5,%ax	# error 5: access denied
	stc
	jmp	l_21_exit

l_21_48:	# DOS 2+ - ALLOCATE MEMORY
	movw	$0, %bx
	movw	$8, %ax # err: insuff. memory
	stc
	jmp	l_21_exit

l_21_49:	# DOS 2+ - FREE MEMORY
	movw	%ES,%ax
	stc
	movw	$7,%ax	# error: memory block destroyed
	jmp	l_21_exit

l_21_4a:	# DOS 2+ - RESIZE MEMORY BLOCK
	clc
	movw	$1,%bx
	jmp	l_21_exit

l_21_4b:	# DOS 2+ - EXEC - LOAD AND/OR EXECUTE PROGRAM
	movw	$5,%ax	# err: file not found
	stc
	jmp	l_21_exit

l_21_4c:	# DOS 2+ - EXIT - TERMINATE WITH RETURN CODE
	pushw	%CS
	popw	%DS
	movw	$0xFFFE,%SP

    	pushw	%CS
    	popw	%ES # restore kernel ES

    	pushw	%CS
    	popw	%ax
    	movw	%ax,%SS # restore kernel SS

#	call	print_registers
	jmp	main

l_21_50:	# DOS 2+ internal - SET CURRENT PROCESS ID (SET PSP ADDRESS)
	clc
	jmp	l_21_exit

l_21_52: # DOS 2+ internal - SYSVARS - GET LIST OF LISTS
	# ничего не делаем! потом разберемся!
	jmp l_21_exit

l_21_55: # DOS 2+ internal - CREATE CHILD PSP
	stc
	jmp	l_21_exit

l_21_58: # DOS 2.11+ - GET OR SET MEMORY ALLOCATION STRATEGY
	movw	$1,%ax # error code "invalid function"
	stc	# set CF flag (error)
	jmp	l_21_exit

l_21_5d:	# DOS 3.1+ internal - SERVER FUNCTION CALL
	jmp	l_21_exit	# ;-)

l_21_60: # DOS 3.0+ - TRUENAME - CANONICALIZE FILENAME OR PATH
	movw	$2,%ax	# err 2: invalid component in directory path or drive letter only
	stc		# err
	jmp	l_21_exit

l_21_unkn_fn:
	  pushw		%CS
	  popw		%DS

	  call	ohw
	  movw	$s_txt21, %si
	  call	sayr_proc
	# getch
	  xorb	%ah,%ah
	  int	$0x16
	# exit to OS
	  jmp	interrupt_90

#l_21_stop:	jmp	l_21_stop
l_21_exit:

	  /* composite IRET ;) */
	
	  popw	%ax
	  popw	%ax
	  popw	%ax	# SP-=6

	  ljmp	*%SS:(%bp)

s_txt21:	.asciz "=INT 21H press anykey "
sayr_proc: # proc       Ver 0.0.2 19-Oct-2004
                        # Процедура вывода строки при помощи функций BIOS
                        # Вход: %DS:%si - ASCIIZ строка.
                        # REGS SAVED !!!
# В графических режимах не работает
	pushw	%ax
	
#       cld
sayr_l1:
        lodsb
        orb     %al,%al
        jz      sayrret
        movb    $0x0e,%ah
        int     $0x10
        jmp     sayr_l1
sayrret:
	popw	%ax
        ret

/* Global variables */

boot_drive:	.word	0
mytimezone:	.word	0
firstboot:	.word	1
drive:		.word	0
reg_bx:		.word	0
reg_cx:		.word	0
reg_dx:		.word	0
cyl:		.word	0
sectors:	.word	0
heads:		.word	0
total_sec:	.word	0

gCyl: .word	0,0,0,0
gSec: .word	0,0,0,0
gHeads: .word	0,0,0,0
gTotal: .word	0,0,0,0

/* FCB - file control block */
/* .word = unsigned short int
   .word twice = unsigned int */

FCB:
	.word	0	/* directory block with file */
	.word	0	/* number file record in directory block */
	.word	0	/* current file block */
	.word	0,0	/* offset */
	.word	0	/* file open mode O_CREAT, O_READ, O_APPEND */
	.word	0,0	/* file length */

basic:
	int	$0x18
	ret
peek:
	pushl	%ebp

	movl	%esp,%ebp
	subl	$4,%esp
	movl	8(%ebp),%ebx
	xor	%eax,%eax
	
	push	%ES
	movl	%eax,%ES
	movl	%ES:(%ebx),%eax
	pop	%ES
	
	leave
	ret

/*
set_color:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	movl	8(%ebp), %eax
	movb	%al,global_color
	leave
	ret

get_color:
	xor	%eax,%eax
	movb	global_color,%al
	ret
*/

putch2:
# VIDEO - WRITE CHARACTER AND ATTRIBUTE AT CURSOR POSITION

# AH = 09h
# AL = character to display
# BH = page number (00h to number of pages - 1)
# background color in 256-color graphics modes (ET4000)
# BL = attribute (text mode) or color (graphics mode)
# if bit 7 set in <256-color graphics mode, character is XORed
# onto screen
# CX = number of times to write character
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

putch_tty:
# VIDEO -  TELETYPE OUTPUT

# AH = 0Eh
# AL = character to write
# BH = page number
# BL = foreground color (graphics modes only)
# CX = number of times to write character
	pushl	%ebp
	movl	%esp, %ebp
	subl	$4, %esp
	movl	8(%ebp), %eax
	movb	%al, -4(%ebp)
	movb $0x0E,%ah
	movzbl	-4(%ebp), %edx
	movb %dl,%al
	xorb %bh,%bh
	movw	$1,%cx
	int $0x10
	leave
	ret

putch_color:
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
	                                                                                
#	movb	$3,%bl # prool fool!

	movw	$1,%cx
	movb	$9,%ah
	xorb	%bh,%bh
	int	$0x10
	leave
	ret

kbhit:
	movb	$0x01, %ah
	int	$0x16
#Return:
#ZF set if no keystroke available
#ZF clear if keystroke available
#AH = BIOS scan code
#AL = ASCII character
	jnz	1f
	xorw	%ax,%ax
	ret
1:
	movw	$0x01,%ax
	ret

setpos:
# VIDEO - SET CURSOR POSITION

# AH = 02h
# BH = page number
# 0-3 in modes 2&3
# 0-7 in modes 0&1
# 0 in graphics modes
# DH = row (00h is top)
# DL = column (00h is left)
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

# VIDEO - GET CURSOR POSITION AND SIZE

# AH = 03h
# BH = page number
# 0-3 in modes 2&3
# 0-7 in modes 0&1
# 0 in graphics modes

# Return:
# AX = 0000h (Phoenix BIOS)
# CH = start scan line
# CL = end scan line
# DH = row (00h is top)
# DL = column (00h is left)

get_row:
	movb	$3,%ah
	xorb	%bh,%bh
	int	$0x10
	xor	%eax,%eax
	movb	%dh,%al
	ret

get_col:
	movb	$3,%ah
	xorb	%bh,%bh
	int	$0x10
	xor	%eax,%eax
	movb	%dl,%al
	ret

/* eto ne rabotaet
getpos:
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

/*
cls_:
# VIDEO - SCROLL UP WINDOW

# AH = 06h
# AL = number of lines by which to scroll up (00h = clear entire window)
# BH = attribute used to write blank lines at bottom of window
# CH,CL = row,column of window's upper left corner
# DH,DL = row,column of window's lower right corner

	movb	$6,%ah
	xorb	%al,%al
	movb	global_color,%bh
	movw	$0x0000,%cx
	movw	$0x184F,%dx
	int	$0x10
	ret

scroll:
	movb	$6,%ah
	movb	$1,%al
	movb	global_color,%bh
	movw	$0x0000,%cx
	movw	$0x184F,%dx
	int	$0x10
	ret
*/
readboot:
	pushl	%ebp

	movl	%esp, %ebp
	movl	8(%ebp), %ebx # parameter: addr of buffer
	
	# ES:BX -> data buffer
	
	movw	$0x0201,%ax # ah = 02 (command 'read'), al=1 - number of sectors to read
	movw	$0x0001,%cx
	# CH = low eight bits of cylinder number
	# CL = sector number 1-63 (bits 0-5) high two bits of cylinder (bits 6-7, hard disk only)
	movw	$0x0000,%dx # DH = head number DL = drive number (bit 7 set for hard disk)

	int	$0x13

	popl	%ebp
	ret

readsec0: # unsigned short int readsec0(char drive, char sec, char head, char trk (or cyl), char *Buffer)
	pushl	%ebp

	movl	%esp,%ebp
	movb	8(%ebp),%dl # drive
	movb	12(%ebp),%cl # sector
	movb	16(%ebp),%dh # head
	movb	20(%ebp),%ch # trk (cyl)
	movw	24(%ebp),%bx # Buffer address

	push	%ES

	push	%DS
	pop	%ES	# ES :=DS

	movw	$0x0201,%ax # ah = 02 (command 'read'), al=1 - number of sectors to read
	int	$0x13
/*
	jnc	1f
	xorw	%ax,%ax
1:
*/

	pop	%ES
	popl	%ebp
	ret

writesec0: # unsigned short int writesec0(char drive, char sec, char head, char trk /* or cyl */, char *Buffer)
	pushl	%ebp

	movl	%esp,%ebp
	movb	8(%ebp),%dl # drive
	movb	12(%ebp),%cl # sector
	movb	16(%ebp),%dh # head
	movb	20(%ebp),%ch # trk (cyl)
	movw	24(%ebp),%bx # Buffer address

	push	%ES
	
	push	%DS
	pop	%ES	# ES :=DS

	movw	$0x0301,%ax # ah = 03 (command 'write'), al=1 - number of sectors to write
	int	$0x13
/*
	jc	1f
	jmp	2f
1:	movw	$0xDEAD,%ax
	
2:
*/
	pop	%ES
	popl	%ebp
	ret
#endif

peek2: # peek2 (segment, offset)
	pushl	%ebp
	
	movl	%esp,%ebp
	push	%ES
	
	movw	8(%ebp),%ax
	movw	%ax,%ES
	movw	12(%ebp),%bx
	movw	%ES:(%bx),%ax
	
	pop	%ES
	popl	%ebp
	ret
	
poke: # poke (value, segment, offset)
	pushl	%ebp
	
	movl %esp,%ebp
	push	%ES
	
	movb	8(%ebp),%al # value
	movw	12(%ebp),%bx # segment
	movw	%bx,%ES
	movw	16(%ebp),%bx # offset
	movb	%al,%ES:(%bx)
	
	pop	%ES
	popl	%ebp
	ret

videomod:
	pushl	%ebp

	movl	%esp, %ebp
	movw	8(%ebp),%ax 

	int	$0x10

	popl	%ebp
	ret

/************************************************************************************/
/*
GetDriveParam: # GetDriveParam(char drive)
	pushl	%ebp

	movl	%esp,%ebp
	push	%ES

	movb	8(%ebp),%dl # drive
	movb	$8,%ah # function
	int	$0x13
	movw	%ax,reg_ax
	movw	%bx,reg_bx
	movw	%cx,reg_cx
	movw	%dx,reg_dx
	movw	%es,reg_es
	movw	%di,reg_di
	jc	1f
	xorw	%ax,%ax
	jmp	9f
1:	
	movw	$0xFFFF,%ax

9:
	pop	%ES
	popl	%ebp
	ret
*/
/************************************************************************************/
GetDriveParam_bx: # GetDriveParam(char drive)
	pushl	%ebp

	movl	%esp,%ebp
	push	%ES

	movb	8(%ebp),%dl # drive
	movb	$8,%ah # function
	int	$0x13
	movw	%bx, %ax # return value
	jc	1f
	jmp	9f
1:
	movw	$0xFFFF,%ax
9:
	pop	%ES
	popl	%ebp
	ret
/************************************************************************************/
/************************************************************************************/
GetDriveParam_cx: # GetDriveParam(char drive)
	pushl	%ebp

	movl	%esp,%ebp
	push	%ES

	movb	8(%ebp),%dl # drive
	movb	$8,%ah # function
	int	$0x13
	movw	%cx, %ax # return value
	jc	1f
	jmp	9f
1:
	movw	$0xFFFF,%ax
9:
	pop	%ES
	popl	%ebp
	ret
/************************************************************************************/
/************************************************************************************/
GetDriveParam_dx: # GetDriveParam(char drive)
	pushl	%ebp

	movl	%esp,%ebp
	push	%ES

	movb	8(%ebp),%dl # drive
	movb	$8,%ah # function
	int	$0x13
	movw	%dx, %ax # return value
	jc	1f
	jmp	9f
1:
	movw	$0xFFFF,%ax
9:
	pop	%ES
	popl	%ebp
	ret
/************************************************************************************/
end_of: # short int end_of (void)
	lea	EndOfCT,%ax
	ret

get_sp: # short int get_ip(void)
	movw	%sp,%ax
	ret

get_rtc:
	movb	$0x02, %ah
	int $0x1A
	movw	%cx, %ax
	ret

get_day:
	movb	$0x04, %ah
	int $0x1A
	movw	%dx, %ax
	ret

get_year:
	movb	$0x04, %ah
	int $0x1A
	movw	%cx, %ax
	ret

reboot:
        movw    $0x40,%ax
        movw    %ax,%ds
        movw    $0x72,%bx
        movw    $0x1234,(%bx)
        .byte   0xea            # JMP   F000:FFF0
        .word   0xfff0,0xf000

cold:
        movw    $0x40,%ax
        movw    %ax,%ds
        movw    $0x72,%bx
        movw    $0x4321,(%bx)
        .byte   0xea            # JMP   F000:FFF0
        .word   0xfff0,0xf000

hdd0:
        xorw    %ax,%ax
        movw    %ax,%ES
        movw    %ax,%DS

        # Read MBR
        movw    $0x0080,%dx  # dl-drive, dh-head
l_read_bootsec:
	movw	$0x0001,%cx  # cl - sector, ch - track

	movw	$0x0201,%ax  # al - sectors count, ah - function (read)

        movw     $0x7c00,%bx
        int     $0x13
        nop
        jc      p_reboot

        # jmp    0:7c00
        .byte      0xea
        .word      0x7c00
        .word      0

p_reboot:# jmp  ffff:0	# ffff:0 = ffff0
        .byte      0xea
        .word      0
        .word      0xffff
    
hdd1:
        xorw    %ax,%ax
        movw    %ax,%ES
        movw    %ax,%DS

        # Read MBR
        movw    $0x0081,%dx  # dl-drive, dh-head
	jmp	l_read_bootsec
    
fdd:
        xorw    %ax,%ax
        movw    %ax,%ES

        # Read boot sector
        movw    $0x0000,%dx  # dl-drive, dh-head
	jmp	l_read_bootsec

/*
print_reg2: # proc    ; ГЛЮЧИТ! по ret не выходит (скорее всего что-то со стеком)
        pushw %ax

	  movb $0x0e,%ah
	  movb $'R',%al
	  int  $0x10	# putch
        call    ohw

	  movb $0x0e,%ah
	  movb $'-',%al
	  int  $0x10	# putch
        pushw   %CS
        popw    %ax
        call    ohw

	  movb $0x0e,%ah
	  movb $'-',%al
	  int  $0x10	# putch
	call next
next:
	popw	%ax
	call	ohw

	  movb $0x0e,%ah
	  movb $'-',%al
	  int  $0x10	# putch
        movw    %SS,%ax
        call    ohw

	  movb $0x0e,%ah
	  movb $'-',%al
	  int  $0x10	# putch
        movw     %SP,%ax
        call    ohw

	  movb $0x0e,%ah
	  movb $'-',%al
	  int  $0x10	# putch
	movw	%BP,%ax
	call	ohw

	  movb $0x0e,%ah
	  movb $'-',%al
	  int  $0x10	# putch
        movw     %DS,%ax
        call    ohw

	  movb $0x0e,%ah
	  movb $'-',%al
	  int  $0x10	# putch
        movw     %ES,%ax
        call    ohw

	  movb $0x0e,%ah
	  movb $'-',%al
	  int  $0x10	# putch
        movw     %bx,%ax
        call    ohw

	  movb $0x0e,%ah
	  movb $'-',%al
	  int  $0x10	# putch
        movw     %cx,%ax
        call    ohw

	  movb $0x0e,%ah
	  movb $'-',%al
	  int  $0x10	# putch
        movw     %dx,%ax
        call    ohw

	  movb $0x0e,%ah
	  movb $'-',%al
	  int  $0x10	# putch
        movw     %si,%ax
        call    ohw

	  movb $0x0e,%ah
	  movb $'-',%al
	  int  $0x10	# putch
        movw     %di,%ax
        call    ohw

        popw    %ax
        ret
*/

ohd: # ÷Ù×ÏÄ ÄÌÉÎÎÏÇÏ ÓÌÏ×Á (%eax, ÔÏ ÅÓÔØ 8 ÂÁÊÔ) × hex ×ÉÄÅ
	# òÅÇÉÓÔÒÙ ÓÏÈÒÁÎÅÎÙ
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

#       ÷Ù×ÏÄ ÓÌÏ×Á × HEX-×ÉÄÅ. ÷ÈÏÄ: ÓÌÏ×Ï × %ax.
#       ÷ÓÅ ÒÅÇÉÓÔÒÙ ÓÏÈÒÁÎÑÀÔÓÑ.
#       ÷ÙÚÙ×ÁÅÔ ÐÏÄÐÒÏÇÒÁÍÍÕ ohb

        pushw    %ax      # óÏÈÒ. ÒÁÄÉ %al.
        movb     %ah,%al
        call    ohb
        popw     %ax              # ÷ÏÓÓÔ. %al.
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

get_boot_drive:
	movw	boot_drive,%ax
	ret

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
