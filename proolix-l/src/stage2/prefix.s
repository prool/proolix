# assembler prefix file for c-translated text

# some comments from Ralf Brown Interrupt list http://www.ctyme.com/intr/int.htm
# Interrupt list rulez! prool

	.text
	.code16gcc
_start:
	.globl	_start

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
	  movb $'#',%al
	  int  $0x10	# putch

	jmp	main

# variables
# human readable magick string

	.ascii	" CT-Kernel ;-) "
/*
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
errno:		.word	0

current_drive:	.byte	0
*/

/* Global variables */

boot_drive:	.word	0

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
