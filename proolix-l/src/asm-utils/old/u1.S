# Proolix-l first assembler test utility

/* Пример ассемблерной утилиты с системным вызовом при помощи прерывания 91 и закоментареным системным
вызовом по методу ljmp/ljmp */

    .code16

    .include "macros.S"

    .global	_start

_start:

    putch $'1'
    putch $'2'
    putch $'3'

    print str1

    call	print_registers

    int		$0x91

    putch $'!'

    call	print_registers

/* Это пример системного вызова по "дурацкой" технологии ljmp/ljmp

Обращение к ядру идет в виде ljmp на адрес обработчика 92h прерывания, но
перед этим в адрес 93h прерывания кладется адрес возврата, куда ядро потом
возвращает управление командой ljmp. При этом не используется стек и соответственно
невозможны вложенные системные вызовы. Но они и так не нужны согласно архитектуре

    # intercept of int 93h
    pushw	%ES
    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0
    pushw	%CS
    popw	%ax
    movw	$0x93*4+2,%si
    movw	%ax,%ES:(%si)
    lea		l_return,%ax
    decw	%si
    decw	%si
    movw	%ax,%ES:(%si)
    popw	%ES

    xorw	%ax,%ax
    movw	%ax,%ES # ES:=0

    movw	$0x248,%si
    ljmp	*%ES:(%si)

l_return:

    putch $'!'

    call	print_registers
    call	print_registers
    call	print_registers
*/

#    int		$0x21

    int		$0x90

	.include "sayr.S"
	.include "ohw.S"
	.include "saycsip.S"

str1:	.ascii	" First assembler utility for OS Proolix"
	.byte	13,10,0
