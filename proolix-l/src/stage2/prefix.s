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

    # intercept of int 4h
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

    # intercept of int 90h
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

/*
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


    # intercept of int 0h
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

    # intercept of int 1h
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

    # intercept of int 2h
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

    # intercept of int 3h
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

    # intercept of int 5h
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

    # intercept of int 6h
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

    # intercept of int 7h
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

    # intercept of int ah
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

    # intercept of int bh
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

    # intercept of int ch
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

    # intercept of int dh
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

run:
        # EXEC!

        movw    $0x4050,%ax	# segment
        movw    %ax,%DS
	movw	%ax,%ES
        cli
        movw    %ax,%SS
        movw    $0xfffe,%SP
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
        movw    $0xfffe,%SP
        sti

        .byte      0xea    # JMP stage2_seg:0100
        .word      0x0100,0x4050

interrupt_90:	# Intercept of some interrupts

	pushw	%CS
	popw	%DS	# DS:=CS

	pushw	%CS
	popw	%ax
	movw	%ax,%ES
	cli
	movw	%ax,%SS
	movw	$0xFFFD,%SP
	sti

	print s_interrupt_90

	jmp	main

interrupt_0:	# Intercept of some interrupts
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_0

	popw	%DS
	iret

s_interrupt_0:	.asciz	" Int 0 "

interrupt_1:	# Intercept of some interrupts
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_1

	popw	%DS
	iret

s_interrupt_1:	.asciz	" Int 1 "

interrupt_2:	# Intercept of some interrupts
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_2

	popw	%DS
	iret

s_interrupt_2:	.asciz	" Int 2 "

interrupt_3:	# Intercept of some interrupts
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_3

	popw	%DS
	iret

s_interrupt_3:	.asciz	" Int 3 "

interrupt_4:	# Intercept of some interrupts
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_4

	popw	%DS
	iret

s_interrupt_4:	.asciz	" Int 4 "

interrupt_5:	# Intercept of some interrupts
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_5

	popw	%DS
	iret

s_interrupt_5:	.asciz	" Int 5 "

interrupt_6:	# Intercept of some interrupts
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_6

	popw	%DS
	iret

s_interrupt_6:	.asciz	" Int 6 "

interrupt_7:	# Intercept of some interrupts
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_7

	popw	%DS
	iret

s_interrupt_7:	.asciz	" Int 7 "

interrupt_a:	# Intercept of some interrupts
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_a

	popw	%DS
	iret

s_interrupt_a:	.asciz	" Int a "

interrupt_b:	# Intercept of some interrupts
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_b

	popw	%DS
	iret

s_interrupt_b:	.asciz	" Int b "

interrupt_c:	# Intercept of some interrupts
	pushw	%DS

	pushw	%CS
	popw	%DS	# DS:=CS

	print s_interrupt_c

l_int_c_stop:	jmp l_int_c_stop

	popw	%DS
	iret

s_interrupt_c:	.asciz	" Int C "

interrupt_d:	# Intercept of some interrupts
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

int80p:		# Proolix-l interrupt (Proolix-l system call)
	pushw	%DS

	pushw	%ax
	putch	$'D'
	putch	$'S'
	putch	$'='
	pushw	%DS	# ax:=DS
	popw	%ax
	call	ohw
	popw	%ax

	pushw	%CS	# DS:=CS
	popw	%DS

	call	print_registers
	popw	%DS
	iret

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

int21p:
	# !!!!!!!!!!!!!!!!!!!!!!!!!!!
	pushw	%DS
	pushw	%ax

	pushw	%CS
	popw	%DS # DS:=CS

	call	ohw

	  movb $0x0e,%ah
	  movb $'=',%al
	  int  $0x10	# putch

	popw	%ax
	popw	%DS
	iret

    pushw	%DS

	pushw	%cx
	pushw	%DS
	popw	%cx

    pushw	%CS
    popw	%DS # DS:=CS

	movw	%cx,DOS_DS
	popw	%cx

# int21h (MSDOS system call) commands (in AH register):

# AH=0 DOS 1+ - TERMINATE PROGRAM

    orb		%ah,%ah
    jz		l_msdos_exit

# AH=9 DOS 1+ - WRITE STRING TO STANDARD OUTPUT
	cmpb	$0x9,%ah
	jz	l_dos_9

# AH=19 DOS 1+ - GET CURRENT DEFAULT DRIVE
	cmpb	$0x19,%ah
	jz	l_dos_19

# AH=25 DOS 1+ - SET INTERRUPT VECTOR
	cmpb	$0x25,%ah
	jz	l_dos_25

# AH=30 DOS 2+ - GET DOS VERSION
	cmpb	$0x30,%ah
	jz	l_dos_30

# AH=33 DOS 2+ - EXTENDED BREAK CHECKING
    cmpb	$0x33,%ah
    jz		l_dos_33

# AH=35 DOS 2+ - GET INTERRUPT VECTOR
	cmpb	$0x35,%ah
	jz	l_dos_35

# AH=47 DOS 2+ - CWD - GET CURRENT DIRECTORY
	cmpb	$0x47,%ah
	jz	l_dos_47

# AH=58 DOS 5+ - GET OR SET UMB LINK STATE
	cmpb	$0x58,%ah
	jz	l_dos_58

    print	s_int21
    call	print_registers
#    putch $'I'

l_end_of_int21:
    pushw	%CS
    popw	%ES # restore kernel ES

    pushw	%CS
    popw	%ax
    movw	%ax,%SS # restore kernel SS

    jmp main

l_dos_9: # DOS 1+ - WRITE STRING TO STANDARD OUTPUT
	push	%ES
	push	%ax
	push	%cx
	push	%di
	push	%si

	# search '$' and replace to 0
	movb	$'$',%al
	mov	$1000,%cx # 1000 - maxlen
	mov	%dx,%di
	push %DS
	pop  %ES # ES=DS
	cld
1:
	repe scasb
	jcxz	2f	# end of string
	xor	%al,%al
	stosb

	# print:
	movw	%dx,%si
	call	sayr_proc

	pop	%si
	pop	%di
	pop	%cx
	pop	%ax
	pop	%ES
	jmp	l_iret
2:
	print l_longstr
	jmp	l_end_of_int21
l_longstr:	.asciz	" MSDOS: VERY LONG STRING OUTPUT "

l_dos_19: # DOS 1+ - GET CURRENT DEFAULT DRIVE
	movb	DOS_CUR_DRIVE,%al
	jmp	l_iret

l_dos_25:	# DOS 1+ - SET INTERRUPT VECTOR
	push	%ax
	push	%si
	push	%ES

	xorw	%ax,%ax
	movw	%ax,%ES # ES=0

	xorb	%ah,%ah # ah=0, al=interrupt number; ax=interrupt number
	shlw	$0x2,%ax # ax=ax*4
	# ax - address of interrupt vector (offset)

	movw	%ax,%si
	movw	%dx,%es:(%si) # offset := DX

	incw	%si
	incw	%si

	movw	DOS_DS,%ax
	movw	%ax,%es:(%si) # segment := DOS DS

	pop	%ES
	pop	%si
	pop	%ax

	jmp l_iret

l_dos_30: # DOS 2+ - GET DOS VERSION
	movw	$0x0303,%ax
	xorw	%bx,%bx
	xorw	%cx,%cx
	jmp l_iret

l_dos_33: # DOS 2+ - EXTENDED BREAK CHECKING
# AL = subfunction
# 00h get current extended break state
    orb	%al,%al
    jz	1f
# 06h DOS 5+ - GET TRUE VERSION NUMBER
    cmpb $0x06,%al
    jz	2f
# 01h set state of extended Break checking
    cmpb $0x01,%al
    jz	3f
    jmp		l_msdos_unkn_subf
2: # DOS 5+ - GET TRUE VERSION NUMBER
    xor		%bx,%bx
    xor		%dx,%dx
    movb	$0xFF,%al
    jmp		l_iret
3: # set br
    movb	%dl,DOS_BREAK
    jmp l_iret
1: # al=0 get break
    movb	DOS_BREAK,%dl
    jmp		l_iret

l_dos_35:
	push	%ax
	push	%si

	xorw	%bx,%bx
	movw	%bx,%ES # ES=0

	xorb	%ah,%ah # ah=0, al=interrupt number; ax=interrupt number
	shlw	$0x2,%ax # ax=ax*4
	# ax - address of interrupt vector (offset)

	movw	%ax,%si
	movw	%es:(%si),%bx # offset -> bx

	incw	%si
	incw	%si

	movw	%es:(%si),%ax
	movw	%ax,%es # segment -> ES

	pop	%si
	pop	%ax

	jmp l_iret

l_dos_47: # DOS 2+ - CWD - GET CURRENT DIRECTORY
	push	%si
	push	%ES

	movw	DOS_DS,%ax
	movw	%ax,%ES
	movb	$'\',%ES:(%si)
	incw	%si
	movb	$0,%ES:(%si)

	movw	$0x100,%ax

	pop	%ES
	pop	%si

	jmp	l_iret

l_dos_58: # DOS 5+ - GET OR SET UMB LINK STATE
	xor	%al,%al
	xor	%bx,%bx
	jmp	l_iret

l_msdos_exit:
    print s_msdos_exit
    jmp l_end_of_int21

l_msdos_unkn_subf:
    print s_unkn_subf
    call	print_registers
    jmp l_end_of_int21
s_unkn_subf:	.asciz	" MSDOS UNKNOWN SUBFUNCTION "

l_iret:
    popw	%DS
    iret

s_interrupt_90:	.asciz	" Int 90 "

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

# variables

/*MSDOS emu*/
DOS_DS:	.word
DOS_BREAK:	.byte	0
DOS_CUR_DRIVE:	.byte	0 # (00h = A:, 01h = B:, etc)
s_int21: .byte 13,10
	.ascii	" Interrupt 21h! "
	.byte 13,10,0
s_msdos_exit:	.byte 13,10
		.ascii "Program terminated"
		.byte 13,10,0

/* Global variables */

boot_drive:	.word	0
mytimezone:	.word	0

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
   $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
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
