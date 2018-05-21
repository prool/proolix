; first fasm util for proolix
; compiling using fasm
;
; this is msdos-like .com file
; execute in Proolix with command 'rundos'
;
; /Prool
;
	org	100H

	mov	ah,0eh
	mov	al,34
	int	10h

	mov	ah,0eh
	mov	al,34
	int	10h

	int	20h
