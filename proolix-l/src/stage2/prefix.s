# assembler prefix file for c-translated text

# some comments from Ralf Brown Interrupt list http://www.ctyme.com/intr/int.htm
# Interrupt list rulez! /prool

	.text
	.code16gcc
_start:	
	.globl	_start

#if 0 // for debug
	  movb $0x0e,%ah
	  movb $'_',%al
	  int  $0x10	# putch ('A')
#endif
	jmp	main
	
# variables
	.ascii	" CT-Kernel ;-) "
SectorsOnCyl:	.word	0
TrkSecs:	.word	0
HeadCnt:	.word	0
RootBeg:	.word	0
DataStart:	.word	0
MaxSectors:	.word	0,0
ResSecs:	.word	0,0
CluSize:	.word	0
CluSizeBytes:	.word	0
FatSize:	.word	0
RootEnd:	.word	0
MaxClusters:	.word	0
MaxCyl:		.word	0

global_color:	.word	0

reg_ax:		.word	0
reg_bx:		.word	0
reg_cx:		.word	0
reg_dx:		.word	0
reg_si:		.word	0
reg_di:		.word	0
reg_es:		.word	0

current_drive:	.byte	0

FCB:
		.word	0,0,0,0,0,0
		.word	0,0,0,0,0,0
		.word	0,0,0,0,0,0

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

readsec0: # unsigned short int readsec0(char drive, char sec, char head, char trk /* or cyl */, char *Buffer)
	pushl	%ebp

	movl	%esp,%ebp
	movb	8(%ebp),%dl # drive
	movb	12(%ebp),%cl # sector
	movb	16(%ebp),%dh # head
	movb	20(%ebp),%ch # trk (cyl)
	movw	24(%ebp),%bx # Buffer address

	push	%DS
	pop	%ES	# ES :=DS

	movw	$0x0201,%ax # ah = 02 (command 'read'), al=1 - number of sectors to read
	int	$0x13

	popl	%ebp
	ret

peek2: # peek2 (segment, offset)
	pushl	%ebp
	
	movl	%esp,%ebp
	push	%ES
	
	movw	8(%ebp),%ax
	movw	%ax,%ES
	movw	12(%ebp),%bx
	movb	%ES:(%bx),%al
	
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

end_of: # short int end_of (void)
	lea	EndOfCT,%ax
	ret
