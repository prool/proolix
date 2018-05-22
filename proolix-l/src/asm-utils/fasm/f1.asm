; first fasm util for proolix
; compiling using fasm
;
; this is msdos-like .com file
; execute in Proolix with command 'rundos'
;
; test of fn25h of int13h
;
; /Prool
;
	org	100H

	push	ES	; save ES

	push	DS
	pop	ES	; ES:=DS

;	mov	ah,0eh
;	mov	al,'%'
;	int	10h

	mov	ah,9
	mov	dx,string
	int	21h

	mov	ax,2500H
	mov	dl,80H
	mov	bx,buffer
	int	13h
	jc	l_err

	mov	ah,9
	mov	dx,s_ok
	int	21h
l_exit:
	pop	ES	; restore ES

	int	20h

l_err:
	call	ohw

	mov	ah,9
	mov	dx,s_err
	int	21h

	jmp	l_exit

include		"ohw.asm"
include		"ohb.asm"

string:		db	"This is 1st fasm .com util for Proolix"
		db	13,10,"$"

s_ok:	db	"int13h OK$"

s_err:	db	" - int13h err$"

buffer:
