;╔══════════════════════════════════════════════════════╗
;║							║
;║	(c) G.Shepelev		All right reserved	║
;║							║
;╠══════════════════════════════════════════════════════╣
;║							║
;║			FONT.COM			║
;║							║
;║	V 0.0			Proolix-версия		║
;║							║
;╚══════════════════════════════════════════════════════╝

;	Загрузчик русского шрифта для Proolix		(c) 1997 G.Shepelev
; Работает только на EGA и VGA!

; Поскольку программа сделана для русификации, то и комментарии на русском...

  IDEAL			; Пользуйтесь TASM в режиме IDEAL !	;-)


MACRO	JR  val	; короткий переход, чтобы TASM не лажался при оптимизации
	jmp  short  val
   ENDM

; Определения для Proolix

INT_NO	equ	84h

exit_vect EQU	0Ch	; вектор процедуры завершения

puts	EQU	7	; выдать символы ASCIIZ в stdout и перевести строку
open	EQU	24h	; открыть файл
read	EQU	26h	; прочитать из файла в буфер
close	EQU	28h	; закрыть файл

O_RDONLY  EQU	1	; режим доступа - только чтение


SEGMENT	seg_$	PARA	PUBLIC 'code'

  ASSUME  CS:seg_$, DS:seg_$, ES:seg_$

	ORG	100h
 

  PROC	font	FAR	; эта процедура "дальняя" в терминах
				; ядра Proolix
start:				; пусть так считается и дальше...
	JR	real_start
	db	0DEh,0ADh	;  signature (magick)
 ENDP


  PROC	real_start	NEAR	; ну а мы весь код сделаем в "ближних" адресах!

	mov	AX,offset txt_c	; адрес начального сообщения
	push	DS		; seg 	(bug of C-compiler)
	push	AX		; offset txt
	call	_puts		; Вывод начального сообщения
	add	SP,4

; определить размер литерной матрицы
	push	DS
	xor	AX,AX
	mov	DS,AX
	mov	AX,[485h]	; POINTS    ( 0040:0085 в реальных адресах )
	pop	DS

	or	AH,AH		; что у нас со старшим байтом POINTS ?
	jz	nxt_test	; продолжим проверку...

err_mode:		; что это за символьная матрица такая? ;)
	mov	AX,offset txt_err_mode	; адрес сообщения об ошибке видеорежима
	JR	d_exit

nxt_test:
	cmp	AL,8	; может у нас 8 строчек в матрице?
	jnz	nxt0
		; режим с 8 строками в матрице
	mov	DX,offset mode8
	mov	CX,2048
	JR	op_file

nxt0:	cmp	AL,14	; а может с 14? ;)
	jnz	nxt1
		; режим с 14 строками в матрице
	mov	DX,offset mode14
	mov	CX,3584
	JR	op_file


nxt1:	cmp	AL,16	; ну тогда уж точно 16!
	jnz	err_mode
		; режим с 16 строками в матрице
	mov	DX,offset mode16
	mov	CX,4096


op_file:		; DX - адрес имени соответствующего файла
			; CX - длина файла
	mov	[f_size],AL	; число строк в символьной матрице

	; открытие файла знакогенератора
	mov	AL,O_RDONLY	; только чтение
	push	AX		; атрибут доступа
	push	DS		; seg	(bug of C-compiler)
	push	DX		; offset path_name
	call	_open
	add	SP,6

	mov	[n_file],AX	; номер открытого файла знакогенератора
	inc	AX		; 0 - ошибка открытия файла
	jnz	open_ok		; открыли - продолжим

d_err:			; что-то не в порядке с дисковой операцией :-(
	mov	AX,offset txt_derr	; адрес сообщения о дисковой ошибке

d_exit:	push	DS		; seg	(bug of C-compiler)
	push	AX		; offset txt
	call	_puts
	add	SP,4
	jmp	[dword ptr exit_vect]	; межсегментный выход из программы


open_ok:	; CX - длина файла
	mov	BX,200h		; длина сектора
	mov	BP,offset buff	; адрес создаваемой таблицы
	mov	SI,BP		; он же адрес начала буфера
rd_file:
	xor	AX,AX
	push	AX		; старшее слово длины (bug of C-compiler)
	push	BX		; длина читаемой порции...
	push	DS		; seg (bug of C-compiler)
	push	SI		; offset	адрес буфера
	push	[n_file]	; номер файла
	call	_read	; посекторное чтение файла (bug of Proolix)
	add	SP,10

	or	DX,DX		; старший байт длины должен быть = 0
	jz	nxt2	; правильно...

cl_d_err:		; ошибка чтения		:-(
	call	n_close		; закрыть файл и выдать сообщение об ошибке
	JR	d_err

nxt2:	cmp	AX,BX	; а весь сектор считан?
	jnz	cl_d_err	; введено не запрошенное число байт

	add	SI,BX	; коррекция позиции в буфере
	sub	CX,BX	; остаток файла
	jnz	rd_file		; продолжение чтения (bug of Proolix)

		; загрузка образов символов в видеопамять
	mov	AX,1100h	; функция 0 - загрузка пользовательского шрифта
			; ES:BP - адрес таблицы
	mov	BH,[f_size]	; число строк в матрице
	mov	CX,256		; число символов в таблице
	xor	DX,DX		; номер первого определяемого символа
	mov	BL,DL		; номер банка знакогенератора
	int	10h		; EGA и VGA сразу делают этот банк активным

			; завершение работы...
	call	n_close		; закрыть файл...
ok:	mov	AX,offset txt_ok	; текст сообщения об успешной загрузке
d_e:	JR	d_exit


n_close:
	push	[n_file]	; номер файла
	call	_close
	inc	sp
	inc	sp
	ret


_close:	mov	AX,close
	JR	f_proolix

_puts:	mov	AX,puts

f_proolix:		; общая точка всех системных вызовов вызовов 
	int	INT_NO
	ret	; жаль, но невозможно здесь удалить параметры со стека
		; (неизвестно сколько их было)

_open:	mov	AX,open
	JR	f_proolix

_read:	mov	AX,read
	JR	f_proolix


txt_c	DB	"Font loader for EGA & VGA  Ver 0.0  (c) 1997 G.Shepelev",0
txt_err_mode	DB	"Incorrect videomode!",0
txt_derr	DB	"Disk error!",0
txt_ok	DB	"Font installed",0

mode8	DB	"08.fnt",0	; имена файлов с образами шрифтов
mode14	DB	"14.fnt",0
mode16	DB	"16.fnt",0

n_file	DW	?	; номер файла
f_size	DB	?	; число строк в матрице символа
buff	DB	16*256 DUP (?) ; максимальный размер файла знакогенератора

 ENDP
	  ENDS

	END	start
