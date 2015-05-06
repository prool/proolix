# Proolix-l stage2 main program
 
        .code16
	.include "macros.S"
	
	
STRLEN	=	80
Drive	=	0	# Загрузочный диск A: (FDD 0, /dev/fd0) 
KernelSeg	=	0x60
	
	.text
.global _start
_start:

    putch	$'-'
    print	str1
    call	print_registers
    newline
    # command cycle
l_revert:
    print prompt
    
    movw	$keystroke,%bx
    call	getstr
    
    # analiz of keystroke
    
    cmpb $0,keystroke
    je l_revert # empty keystroke
    
    ### command c1
    xorw %bx,%bx
c1_0:  
    movb c1_s(%bx),%al
    cmpb %al,keystroke(%bx)
    jne	c1_1
    mov keystroke(%bx),%dl
    orb %al,%dl
    jz c1_2
    incw %bx
    jmp c1_0
c1_2:	# this command
    print s_help
    jmp l_revert
c1_1:	#    no this command
    ###
    
    ### command c2
    xorw %bx,%bx
c2_0:  
    movb c2_s(%bx),%al
    cmpb %al,keystroke(%bx)
    jne	c2_1
    movb keystroke(%bx),%dl
    orb %al,%dl
    jz c2_2
    incw %bx
    jmp c2_0
c2_2:	# this command
        movw    $0x40,%ax
        movw    %ax,%ds
        movw    $0x72,%bx
        movw    $0x1234,(%bx)

        .byte   0xea            # JMP   F000:FFF0
        .word   0xfff0,0xf000
	
c2_1:	#    no this command
    ###
    
    ### command c3
    xorw %bx,%bx
c3_0:  
    movb c3_s(%bx),%al
    cmpb %al,keystroke(%bx)
    jne	c3_1
    movb keystroke(%bx),%dl
    orb %al,%dl
    jz c3_2
    incw %bx
    jmp c3_0
c3_2:	# this command
        movw    $0x40,%ax
        movw    %ax,%ds
        movw    $0x72,%bx
        movw    $0x4321,(%bx)

        .byte   0xea            # JMP   F000:FFF0
        .word   0xfff0,0xf000
	
c3_1:	#    no this command
    ###
    
    ### command c4
    xorw %bx,%bx
c4_0:  
    movb c4_s(%bx),%al
    cmpb %al,keystroke(%bx)
    jne	c4_1
    movb keystroke(%bx),%dl
    orb %al,%dl
    jz c4_2
    incw %bx
    jmp c4_0
c4_2:	# this command
        xorw    %ax,%ax
        movw    %ax,%ES

        # Read MBR
        movw    $0x0080,%dx  # dl-drive, dh-head
l_read_mbr_:
	movw	$0x0001,%cx  # cl - sector, ch - track

	movw	$0x0201,%ax  # al - sectors count, ah - function (read)

        movw     $0x7c00,%bx
        int     $0x13
        nop
        jc      c4_reboot

        # jmp    0:7c00
        .byte      0xea
        .word      0x7c00
        .word      0

c4_reboot:# jmp  ffff:0	# ffff:0 = ffff0
        .byte      0xea
        .word      0
        .word      0xffff
	
c4_1:	#    no this command
    ###
    
    ### command c5
    xorw %bx,%bx
c5_0:  
    movb c5_s(%bx),%al
    cmpb %al,keystroke(%bx)
    jne	c5_1
    movb keystroke(%bx),%dl
    orb %al,%dl
    jz c5_2
    incw %bx
    jmp c5_0
c5_2:	# this command
        xorw    %ax,%ax
        movw    %ax,%ES

        # Read MBR
        movw    $0x0081,%dx  # dl-drive, dh-head
	jmp	l_read_mbr_
	
c5_1:	#    no this command
    ###
    
    ### command c6
    xorw %bx,%bx
0:  
    movb c6_s(%bx),%al
    cmpb %al,keystroke(%bx)
    jne	8f
    movb keystroke(%bx),%dl
    orb %al,%dl
    jz 1f
    incw %bx
    jmp 0b
1:	# this command
    #
	newline
        # Вычисления
        movb    FatCnt,%al
        cbw
        movw    FatSize,%cx
	mulw    %cx      # %ax*%cx -> %dx:%ax
        addw    ResSecs,%ax
        # %ax - Root Beginning Sector
        movw    %ax,StartClu
        PUSHW   %ax

        movw    RootSiz,%ax
        movw    $32,%cx
        mulw    %cx

        movw    SectSiz,%cx
        divw    %cx       # %dx:%ax / %cx -> %ax; mod -> %dx
        orw     %dx,%dx   # Если остаток ненулевой, то прибавляем еще один
        jz      2f        # сектор (последний неполный сектор root'а)
        incw    %ax
2:
        # %ax - Root Size in Sectors
        addw    %ax,StartClu
        movw    %ax,%cx
        movw    %ax,RootSize
	    # print	s_lines
	    # putch	$':'
	    # movw		%cx,%ax
	    # call	ohw
        POPW    %ax      # %ax - Root Bg Sec
	    # putch	$','
	    # call	ohw
3:      # Загрузка корневого каталога посекторно и посекторный вывод списка
	# файлов
        pushw   %cx

        pushw   %ax
        movw    $Buff,%bx
        movb    $Drive,%dl
        pushw   %ax
        call    SecRead
        jnc     4f
	putch	$'E' # error
4:	
        popw    %ax
        movw    $16,%cx   # root entryes in 1 sector
5:              pushw   %cx
		pushw	%bx

		movb	(%bx),%al
		orb	%al,%al
		jz	7f
		cmpb	$0xE5,%al
		je	7f
                movw    $11,%cx

6:          	putch	(%bx)
			incw	%bx		
			loop	6b
		
		newline
7:		
		popw	%bx
                popw    %cx
                addw    $32,%bx
                loop    5b

        popw    %ax
        incw    %ax

        popw    %cx
	# putch	$'s'
        loop    3b
        # end of load root dir
	jmp	l_revert
     
8:	#    no this command
    ###
    
    
    ### command c7
    xorw %bx,%bx
0:  
    movb c7_s(%bx),%al
    cmpb %al,keystroke(%bx)
    jne	8f
    movb keystroke(%bx),%dl
    orb %al,%dl
    jz 1f
    incw %bx
    jmp 0b
1:	# this command
    #
    print	s_filename
    movw	$Filename,%bx
    call	getstr
	newline
        # Вычисления
        movb    FatCnt,%al
        cbw
        movw    FatSize,%cx
	mulw    %cx      # %ax*%cx -> %dx:%ax
        addw    ResSecs,%ax
        # %ax - Root Beginning Sector
        movw    %ax,StartClu
        PUSHW   %ax

        movw    RootSiz,%ax
        movw    $32,%cx
        mulw    %cx

        movw    SectSiz,%cx
        divw    %cx       # %dx:%ax / %cx -> %ax; mod -> %dx
        orw     %dx,%dx   # Если остаток ненулевой, то прибавляем еще один
        jz      2f        # сектор (последний неполный сектор root'а)
        incw    %ax
2:
        # %ax - Root Size in Sectors
        addw    %ax,StartClu
        movw    %ax,%cx
        movw    %ax,RootSize
	    # print	s_lines
	    # putch	$':'
	    # movw		%cx,%ax
	    # call	ohw
        POPW    %ax      # %ax - Root Bg Sec
	    # putch	$','
	    # call	ohw
3:      # Загрузка корневого каталога посекторно и посекторный поиск в нем
	# искомого файла
        pushw   %cx

        pushw   %ax
        movw    $Buff,%bx
        movb    $Drive,%dl
        pushw   %ax
        call    SecRead
        jnc     4f
	putch	$'E' # error
4:	
        popw    %ax
        movw    $16,%cx   # root entryes in 1 sector
5:              pushw   %cx
		pushw	%bx

		movb	(%bx),%al
		orb	%al,%al
		jz	7f
		cmpb	$0xE5,%al
		je	7f
		# сравниваем найденное имя файла с эталонным
7:		
                movw    $11,%cx
                movw    %bx,%di
		movw    $Filename,%si
                rep     cmpsb
                je      9f
		
		popw	%bx
                popw    %cx
                addw    $32,%bx
                loop    5b

        popw    %ax
        incw    %ax

        popw    %cx
	# putch	$'s'
        loop    3b
        # end of load root dir
	print	s_file_not_found
	jmp	l_revert
	
9:
###===
        # HАШЛИ !
        popw    %cx
        # Found file
        # %bx - file dir record
        # word ptr (%bx+0x1a) - 1st cluster
        movw    0x1a(%bx),%ax    # %ax - 1st cluster

        # Загрузка FAT в ОЗУ
        PUSHW   %ax      # save %ax with 1st clu no

        movw    ResSecs,%ax
        movw    $FATaddr,%bx
        movw    FatSize,%cx

        # Загрузка ядра покластерно (по цепочке из FATа)
        # Загрузка FAT
0:      movb    $Drive,%dl
        pushw   %ax
        call    SecRead
        jnc     1f
	# FAT read error
	print	s_fat_err
	popw	%ax	# restore stack
	jmp	l_revert
1:	
        popw    %ax
        addw    $512,%bx
        incw    %ax      # Next sector!

#       pushw   %ax
#       CHR1    'F'
#       popw    %ax

        loop    0b

        movb    ClustSiz,%al
        cbw
        movw    SectSiz,%dx
        mul     %dx
        movw    %ax,%cx   # Cluster Size in Bytes

        POPW    %ax      # restore %ax (1st clu no)
1:      movw    $Buff,%bx
        movb    $Drive,%dl
        call    CluRead
	# print cluster
	    pushw	%cx
	    pushw	%bx
	    pushw	%ax
	    
2:	    movb	(%bx),%al
	    putch	%al
	    incw	%bx
	    loop	2b
	    
	    popw	%ax
	    popw	%bx
	    popw	%cx
	# end of 'print cluster'
#          pushw   %ax
#	   movw $0x0e00+176,%ax
#          int  $0x10
#          popw    %ax
        call    NextClu # set %ax
        jnc     1b
	# end of file
	jmp	l_revert

###===	
     
8:	#    no this command
    ###
    
    ### command c8
    xorw %bx,%bx
0:  
    movb c8_s(%bx),%al
    cmpb %al,keystroke(%bx)
    jne	8f
    movb keystroke(%bx),%dl
    orb %al,%dl
    jz 1f
    incw %bx
    jmp 0b
1:	# this command
    #
    print	s_filename
    movw	$Filename,%bx
    call	getstr
	newline
        # Вычисления
        movb    FatCnt,%al
        cbw
        movw    FatSize,%cx
	mulw    %cx      # %ax*%cx -> %dx:%ax
        addw    ResSecs,%ax
        # %ax - Root Beginning Sector
        movw    %ax,StartClu
        PUSHW   %ax

        movw    RootSiz,%ax
        movw    $32,%cx
        mulw    %cx

        movw    SectSiz,%cx
        divw    %cx       # %dx:%ax / %cx -> %ax; mod -> %dx
        orw     %dx,%dx   # Если остаток ненулевой, то прибавляем еще один
        jz      2f        # сектор (последний неполный сектор root'а)
        incw    %ax
2:
        # %ax - Root Size in Sectors
        addw    %ax,StartClu
        movw    %ax,%cx
        movw    %ax,RootSize
	    # print	s_lines
	    # putch	$':'
	    # movw		%cx,%ax
	    # call	ohw
        POPW    %ax      # %ax - Root Bg Sec
	    # putch	$','
	    # call	ohw
3:      # Загрузка корневого каталога посекторно и посекторный поиск в нем
	# искомого файла
        pushw   %cx

        pushw   %ax
        movw    $Buff,%bx
        movb    $Drive,%dl
        pushw   %ax
        call    SecRead
        jnc     4f
	putch	$'E' # error
4:	
        popw    %ax
        movw    $16,%cx   # root entryes in 1 sector
5:              pushw   %cx
		pushw	%bx

		movb	(%bx),%al
		orb	%al,%al
		jz	7f
		cmpb	$0xE5,%al
		je	7f
		# сравниваем найденное имя файла с эталонным
7:		
                movw    $11,%cx
                movw    %bx,%di
		movw    $Filename,%si
                rep     cmpsb
                je      9f
		
		popw	%bx
                popw    %cx
                addw    $32,%bx
                loop    5b

        popw    %ax
        incw    %ax

        popw    %cx
	# putch	$'s'
        loop    3b
        # end of load root dir
	print	s_file_not_found
	jmp	l_revert
	
9:
###===
        # HАШЛИ !
        popw    %cx
        # Found file
        # %bx - file dir record
        # word ptr (%bx+0x1a) - 1st cluster
        movw    0x1a(%bx),%ax    # %ax - 1st cluster

        # Загрузка FAT в ОЗУ
        PUSHW   %ax      # save %ax with 1st clu no

        movw    ResSecs,%ax
        movw    $FATaddr,%bx
        movw    FatSize,%cx

        # Загрузка ядра покластерно (по цепочке из FATа)
        # Загрузка FAT
0:      movb    $Drive,%dl
        pushw   %ax
        call    SecRead
        jnc     1f
	# FAT read error
	print	s_fat_err
	popw	%ax	# restore stack
	jmp	l_revert
1:	
        popw    %ax
        addw    $512,%bx
        incw    %ax      # Next sector!

#       pushw   %ax
#       CHR1    'F'
#       popw    %ax

        loop    0b

        movw    $32,%cx   # 16-chars strings in cluster (cluster size=512)

        POPW    %ax      # restore %ax (1st clu no)
1:      movw    $Buff,%bx
        movb    $Drive,%dl
        call    CluRead
	# print cluster
	    pushw	%cx
	    pushw	%bx
	    pushw	%ax
	    
2:	    
	    # print 16 chars in hex mode, %bx - addr
	    
	    call	print16
	    add		$16,%bx
	    
	    loop	2b
	    
	    popw	%ax
	    popw	%bx
	    popw	%cx
	# end of 'print cluster'
#          pushw   %ax
#	   movw $0x0e00+176,%ax
#          int  $0x10
#          popw    %ax
        call    NextClu # set %ax
        jnc     1b
	# end of file
	jmp	l_revert

###===	
     
8:	#    no this command
    ###
    
    
    ### command c9
    xorw %bx,%bx
0:  
    movb c9_s(%bx),%al
    cmpb %al,keystroke(%bx)
    jne	8f
    movb keystroke(%bx),%dl
    orb %al,%dl
    jz 1f
    incw %bx
    jmp 0b
1:	# this command
    #
    print	s_filename
    movw	$Filename,%bx
    call	getstr
	newline
        # Вычисления
        movb    FatCnt,%al
        cbw
        movw    FatSize,%cx
	mulw    %cx      # %ax*%cx -> %dx:%ax
        addw    ResSecs,%ax
        # %ax - Root Beginning Sector
        movw    %ax,StartClu
        PUSHW   %ax

        movw    RootSiz,%ax
        movw    $32,%cx
        mulw    %cx

        movw    SectSiz,%cx
        divw    %cx       # %dx:%ax / %cx -> %ax; mod -> %dx
        orw     %dx,%dx   # Если остаток ненулевой, то прибавляем еще один
        jz      2f        # сектор (последний неполный сектор root'а)
        incw    %ax
2:
        # %ax - Root Size in Sectors
        addw    %ax,StartClu
        movw    %ax,%cx
        movw    %ax,RootSize
	    # print	s_lines
	    # putch	$':'
	    # movw		%cx,%ax
	    # call	ohw
        POPW    %ax      # %ax - Root Bg Sec
	    # putch	$','
	    # call	ohw
3:      # Загрузка корневого каталога посекторно и посекторный поиск в нем
	# искомого файла
        pushw   %cx

        pushw   %ax
        movw    $Buff,%bx
        movb    $Drive,%dl
        pushw   %ax
        call    SecRead
        jnc     4f
	putch	$'E' # error
4:	
        popw    %ax
        movw    $16,%cx   # root entryes in 1 sector
5:              pushw   %cx
		pushw	%bx

		movb	(%bx),%al
		orb	%al,%al
		jz	7f
		cmpb	$0xE5,%al
		je	7f
		# сравниваем найденное имя файла с эталонным
7:		
                movw    $11,%cx
                movw    %bx,%di
		movw    $Filename,%si
                rep     cmpsb
                je      9f
		
		popw	%bx
                popw    %cx
                addw    $32,%bx
                loop    5b

        popw    %ax
        incw    %ax

        popw    %cx
	# putch	$'s'
        loop    3b
        # end of load root dir
	print	s_file_not_found
	jmp	l_revert
	
9:
###===
        # HАШЛИ !
        popw    %cx
        # Found file
        # %bx - file dir record
        # word ptr (%bx+0x1a) - 1st cluster
        movw    0x1a(%bx),%ax    # %ax - 1st cluster

        # Загрузка FAT в ОЗУ
        PUSHW   %ax      # save %ax with 1st clu no

        movw    ResSecs,%ax
        movw    $FATaddr,%bx
        movw    FatSize,%cx

        # Загрузка ядра покластерно (по цепочке из FATа)
        # Загрузка FAT
0:      movb    $Drive,%dl
        pushw   %ax
        call    SecRead
        jnc     1f
	# FAT read error
	print	s_fat_err
	popw	%ax	# restore stack
	jmp	l_revert
1:	
        popw    %ax
        addw    $512,%bx
        incw    %ax      # Next sector!

#       pushw   %ax
#       CHR1    'F'
#       popw    %ax

        loop    0b

        movb    ClustSiz,%al
        cbw
        movw    SectSiz,%dx
        mul     %dx
        movw    %ax,%cx   # Cluster Size in Bytes
	
	movw	$KernelSeg,%ax
	movw	%ax,%ES

        POPW    %ax      # restore %ax (1st clu no)
        xorw    %bx,%bx
1:      movb    $Drive,%dl
        call    CluRead	# read to %ES:%bx
#          pushw   %ax
#	   movw $0x0e00+176,%ax
#          int  $0x10
#          popw    %ax
        call    NextClu # set %ax
        jc      9f
	addw	$512,%bx	# 512 - cluster size in bytes (ad hoc cluster=1 sector)
	jmp	1b
	# end of file
9:	
	# start kernel
        movw    $KernelSeg,%ax
        movw    %ax,%DS
	movw	%ax,%ES
        cli
        movw    %ax,%SS
        movw    $0xfffe,%SP
        sti

        .byte      0xea    # JMP KernelSeg:0000
        .word      0x0000,KernelSeg

###===	
     
8:	#    no this command
    ###
    
    
#invalid command
    
    print s_inv
    
    jmp l_revert

SecRead: # proc    # ver 0.0.1   3-Nov-94 (for FDD boot)

                # Read absolute sectors
                # Input: %ax - abs sec number
                #        %dl - drive (for int 0x13 Fn=2)
                #        ES:%bx - buffer
                #        DS - data segment reg

                # Use global variables:
                # word ptr HeadCnt
                # word ptr TrkSecs

                # Output: cf=1 if error
                # NO ALL REGS SAVED !!!

#       pushw    %ax
        pushw    %bx
        pushw    %cx
        pushw    %dx
        pushw    %si
        pushw    %di
 
        movw    %bx,%si   # save %bx & %dx
        movw    %dx,%di

        PUSHW    %ax

        movw    HeadCnt,%ax
        movw    TrkSecs,%cx
        mulw     %cx      # %dx:%ax := HeadCnt * TrkSecs
        movw    %ax,%bx  # %bx = HeadCnt * TrkSecs

        POPW     %ax
        xorw     %dx,%dx   # %dx:%ax - Abs Sec No
        div     %bx # %ax=Track=AbsSec/(HeadCnt*TrkSecs); %dx - mod (Sec on Cyl)

        movw    %ax,%bx   # %bx - track no

        movw    %dx,%ax
        xorw    %dx,%dx   # %dx:%ax - module (Sec on Cyl)
        div     %cx # Head=[%dx:%ax]/TrkSecs;%ax - Head; mod (%dx) - Sec on Head-1
        incw    %dx # %dx - Sec on Head

        movw    $1,%cx
l_loop:
        pushw    %cx

        movb     %dl,%cl    # sector
        movb     %bl,%ch    # track ; Warning: track < 255 !!!
        movw     %si,%bx    # Restore %bx - offset for buff
        movw     %di,%dx    # Restore %dl - drive no
        movb     %al,%dh    # head
        movw     $0x201,%ax # Fn=02, Read 1 sector
        int      $0x13
        jnc     l_break

        popw     %cx
        loop    l_loop
        jmp     l_ret
l_break:
        popw     %cx
l_ret:
        popw     %di
        popw     %si
        popw     %dx
        popw     %cx
        popw     %bx
#       popw     %ax

        ret
# SecRead endp

CluRead: # proc    # Read cluster
                # Input: %ax - cluster number
                #        %dl - drive (for int 0x13 Fn=2)
                #        ES:%bx - buffer
                #        DS - data segment reg

                # Use global variables:
                # StartClu      .word      0       # Number sector of 1st cluster
                # ClustSiz      .byte      2
                # SectSiz       .word

                # ALL REGS SAVED !!!

        pushw    %ax
        pushw    %bx
        pushw    %cx

        # Sector = StartClu + ((Clu-2) * ClustSiz)
        PUSHW    %bx
        PUSHW    %dx
        decw     %ax      # %ax:=%ax-2
        decw     %ax      #
        movw    %ax,%bx
        movb    ClustSiz,%al
        cbw
        movw    %ax,%cx
        mul     %bx
        addw    StartClu,%ax
        # %ax - sector
        POPW     %dx      # Restore %dl
        POPW     %bx
        # %cx - ClustSiz
loop4:
        pushw    %ax
        call    SecRead
        # jc      l_err
        popw     %ax
        addw    SectSiz,%bx
        incw     %ax
        loop    loop4

        popw     %cx
        popw     %bx
        popw     %ax

        ret
# CluRead endp

NextClu: # proc # Input: %ax - cluster no
                # Output: %ax - next cluster from FAT
                # cf=1 if EOF
                # Use global var:
                # FATaddr               .word      0
                # REGS SAVED !

        pushw    %bx
        pushw    %cx
        pushw    %dx
        pushw    %si
        pushw    %di

        movw    %ax,%di
        movw    $3,%bx
        mul     %bx      # %dx:%ax := %ax * %bx
        shrw    $1,%ax   # %ax := %ax /2
        # %ax - addr in FAT
        movw    %ax,%si
        movw    $FATaddr,%bx
        movw    (%bx,%si),%ax
        testw   $1,%di
        jz      lbl5
        movb    $4,%cl
        shrw    %cl,%ax
        jmp     lbl6
lbl5:   andw    $0xfff,%ax
lbl6:
#       cmpw    $0xfff,%ax
#       je      l_cf
#       cmpw    $0xff7,%ax
#       je      l_cf
        cmpw    TotSecs,%ax
        ja      l_cf # $$$

        clc
l_ret1:
        popw     %di
        popw     %si
        popw     %dx
        popw     %cx
        popw     %bx

        ret
l_cf:   stc
        jmp     l_ret1
# NextClu endp

getstr:	# read of string from console
	# input: %bx - string buffer address

    pushw	%ax
    pushw	%bx
    pushw	%si
    
    movw	%bx,%si	# %si - string buffer address
    xorw %bx,%bx # string index
    
9:

    xorb %ah,%ah
    int $0x16	# getch to %al
    
    cmpb $0x0d,%al
    je	1f
    
    movb %al,(%bx,%si)
    
    putch %al # echo
    
    incw %bx
    
    cmpw $STRLEN,%bx
    je 0f
    jmp 9b
    
0:
    print s_too_len_str
    movb $0,(%si)
    
1:
    movb $0,(%bx,%si)
    
    popw	%si
    popw	%bx
    popw	%ax
    ret
    
# end of getstr

print16:	# print of 16 chars in hex and ascii mode
		# input: %bx - string addr
		# regs saved
    pushw	%ax
    pushw	%bx
    pushw	%cx
    pushw	%dx
    
    movw	%bx,%dx	# save %bx
    
    movw	$16,%cx
1:
    movb	(%bx),%al
    call	ohb
    putch	$' '
    incw	%bx
    loop	1b
    
    putch		$' '
    movw	%dx,%bx # restore %bx
    
    movw	$16,%cx
2:
    movb	(%bx),%al
    cmpb	$' ',%al
    jb		3f
    putch	%al
    jmp		4f
3:  putch	$'.'
4:
    incw	%bx
    loop	2b    
    
    newline
    movw	$keystroke,%bx
    call	getstr
    
    popw	%dx
    popw	%cx
    popw	%bx
    popw	%ax
    
    ret

keystroke: .fill STRLEN,1,0
Filename:  .fill STRLEN,1,0

str1:	.byte		13,10
	.ascii		"Proolix-l stage2. Ver 6-May-2015. Use 'help' for help"
	.byte 		13,10,0
prompt:	.byte	13,10
	.asciz		"> "
s_filename:
	.byte	13,10
	.asciz	"Filename? "	
s_too_len_str: .ascii "Too length string"
	.byte 13,10,0
s_inv:	.byte 13,10
	.ascii "Invalid command"	
	.byte 13,10,0
s_file_not_found:
	.byte 13,10
	.ascii "File not found"	
	.byte 13,10,0
s_fat_err:
	.byte 13,10
	.ascii "FAT read error"	
	.byte 13,10,0
c1_s: .asciz "help"
c2_s: .asciz "reboot"
c3_s: .asciz "cold"
c4_s: .asciz "hdd0"
c5_s: .asciz "hdd1"
c6_s: .asciz "ls"
c7_s: .asciz "cat"
c8_s: .asciz "hcat"
c9_s: .asciz "run"
s_help: .byte 13,10,13,10
.ascii "Proolix-l stage2 commands:"
.byte 13,10
.ascii "help - this help ;)"
.byte 13,10
.ascii "ls - list of files (kernels)"
.byte 13,10
.ascii "reboot - hot reboot"
.byte 13,10
.ascii "cold - cold reboot"
.byte 13,10
.ascii "hdd0 - load OS from HDD 0"
.byte 13,10
.ascii "hdd1 - load OS from HDD 1 (if present)"
.byte 13,10
.ascii "cat - output file to console (text mode)"
.byte 13,10
.ascii "hcat - output file to console (hex mode)"
.byte 13,10
.ascii "run - load and run kernel"
.byte 13,10
.byte 13,10
.ascii "With best regards, Prool. http://prool.kharkov.org/proolix/"
.byte 13,10,0
	
s_debug:	.asciz "DEBUG"	
s_lines:	.asciz "Root size in sectors, root bg sec"	

SectSiz:         .word      512
ClustSiz:        .byte      1
ResSecs:         .word      1
FatCnt:          .byte      2
RootSiz:         .word      224
TotSecs:         .word      2880
FatSize:         .word      9
TrkSecs:         .word      18
HeadCnt:         .word      2
HidnSec:         .word      0,0
BigNo:           .word      0,0       # Big total no of sectors
StartClu:        .word      0       
RootSize:        .word      0
# end of boot.S file
