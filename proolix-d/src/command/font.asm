;浜様様様様様様様様様様様様様様様様様様様様様様様様様様融
;�							�
;�	(c) G.Shepelev		All right reserved	�
;�							�
;麺様様様様様様様様様様様様様様様様様様様様様様様様様様郵
;�							�
;�			FONT.COM			�
;�							�
;�	V 0.0			Proolix-▲珀��		�
;�							�
;藩様様様様様様様様様様様様様様様様様様様様様様様様様様夕

;	���珮х┴ 珮瘁���� 葯�籵� か� Proolix		(c) 1997 G.Shepelev
; ��｀��モ 皰�讓� �� EGA � VGA!

; ��瓷��讓� �牀������ 瓩カ��� か� 珮瓱筥��罔�, 皰 � ����キ��爬� �� 珮瘁���...

  IDEAL			; ���讌礬皀瘡 TASM � 爛Θ�� IDEAL !	;-)


MACRO	JR  val	; ��牀皖┤ �ムュ��, 艪�° TASM �� �����瘴 �爬 ��皋�├�罔�
	jmp  short  val
   ENDM

; ｯ爛ぅ�キ�� か� Proolix

INT_NO	equ	84h

exit_vect EQU	0Ch	; ▲�皰� �牀罐ゃ琺 ��▲琥キ��

puts	EQU	7	; �襪�碎 瓱�〓�� ASCIIZ � stdout � �ムアメ皋 痰牀��
open	EQU	24h	; �皖琺碎 ����
read	EQU	26h	; �牀腮��碎 ├ ����� � ＜筌�
close	EQU	28h	; ���琺碎 ����

O_RDONLY  EQU	1	; 爛Θ� ぎ痰祚� - 皰�讓� 艪キ┘


SEGMENT	seg_$	PARA	PUBLIC 'code'

  ASSUME  CS:seg_$, DS:seg_$, ES:seg_$

	ORG	100h
 

  PROC	font	FAR	; 轤� �牀罐ゃ�� "���讚闖" � 皀爼┃��
				; 錣�� Proolix
start:				; �竅碎 ��� 瘍���モ瘴 � ���跏�...
	JR	real_start
	db	0DEh,0ADh	;  signature (magick)
 ENDP


  PROC	real_start	NEAR	; �� � �� ▲瘡 ��� 瓩カ�ガ � "゛└���" �むメ��!

	mov	AX,offset txt_c	; �むメ �����讚��� 甌�♂キ��
	push	DS		; seg 	(bug of C-compiler)
	push	AX		; offset txt
	call	_puts		; �襤�� �����讚��� 甌�♂キ��
	add	SP,4

; ��爛ぅ��碎 ��Кム ��皀爿�� ��矗�肓
	push	DS
	xor	AX,AX
	mov	DS,AX
	mov	AX,[485h]	; POINTS    ( 0040:0085 � 爛��讚諷 �むメ�� )
	pop	DS

	or	AH,AH		; 艪� � ��� 甌 痰�琥━ ���皰� POINTS ?
	jz	nxt_test	; �牀ぎ�Θ� �牀▲爲�...

err_mode:		; 艪� 轤� �� 瓱�〓�讚�� ��矗��� �����? ;)
	mov	AX,offset txt_err_mode	; �むメ 甌�♂キ�� �� �荐！� ※ぅ�爛Θ��
	JR	d_exit

nxt_test:
	cmp	AL,8	; ��Ε� � ��� 8 痰牀腑� � ��矗�罐?
	jnz	nxt0
		; 爛Θ� � 8 痰牀���� � ��矗�罐
	mov	DX,offset mode8
	mov	CX,2048
	JR	op_file

nxt0:	cmp	AL,14	; � ��Ε� � 14? ;)
	jnz	nxt1
		; 爛Θ� � 14 痰牀���� � ��矗�罐
	mov	DX,offset mode14
	mov	CX,3584
	JR	op_file


nxt1:	cmp	AL,16	; �� 皰��� 礒 皰膈� 16!
	jnz	err_mode
		; 爛Θ� � 16 痰牀���� � ��矗�罐
	mov	DX,offset mode16
	mov	CX,4096


op_file:		; DX - �むメ ━キ� 甌�癶モ痰≪鉗ィ� �����
			; CX - か┃� �����
	mov	[f_size],AL	; 腮甄� 痰牀� � 瓱�〓�讚�� ��矗�罐

	; �皖琺皋� ����� Л����キム�皰��
	mov	AL,O_RDONLY	; 皰�讓� 艪キ┘
	push	AX		; �矗─竄 ぎ痰祚�
	push	DS		; seg	(bug of C-compiler)
	push	DX		; offset path_name
	call	_open
	add	SP,6

	mov	[n_file],AX	; ���ム �皖琺皰�� ����� Л����キム�皰��
	inc	AX		; 0 - �荐！� �皖琺皋� �����
	jnz	open_ok		; �皖琺�� - �牀ぎ�Θ�

d_err:			; 艪�-皰 �� � ��瑙お� � え瓷�〓� ��ム�罔ォ :-(
	mov	AX,offset txt_derr	; �むメ 甌�♂キ�� � え瓷�〓� �荐！�

d_exit:	push	DS		; seg	(bug of C-compiler)
	push	AX		; offset txt
	call	_puts
	add	SP,4
	jmp	[dword ptr exit_vect]	; �ウ瓮��キ皚覃 �諷�� ├ �牀������


open_ok:	; CX - か┃� �����
	mov	BX,200h		; か┃� 瓮�皰��
	mov	BP,offset buff	; �むメ 甌Г���ガ�� ��゛�肓
	mov	SI,BP		; �� Ε �むメ ������ ＜筌��
rd_file:
	xor	AX,AX
	push	AX		; 痰�琥ゥ 甄�〓 か┃� (bug of C-compiler)
	push	BX		; か┃� 腮��ガ�� ��琅┬...
	push	DS		; seg (bug of C-compiler)
	push	SI		; offset	�むメ ＜筌��
	push	[n_file]	; ���ム �����
	call	_read	; ��瓮�皰爿�� 艪キ┘ ����� (bug of Proolix)
	add	SP,10

	or	DX,DX		; 痰�琥┤ ���� か┃� ぎ�Ε� °碎 = 0
	jz	nxt2	; ���※�讚�...

cl_d_err:		; �荐！� 艪キ��		:-(
	call	n_close		; ���琺碎 ���� � �襪�碎 甌�♂キ┘ �� �荐！�
	JR	d_err

nxt2:	cmp	AX,BX	; � ▲瘡 瓮�皰� 瘍����?
	jnz	cl_d_err	; □イキ� �� ���牀茱���� 腮甄� ����

	add	SI,BX	; ��玻オ罔� ��Ж罔� � ＜筌爛
	sub	CX,BX	; �痰�皰� �����
	jnz	rd_file		; �牀ぎ�Ε�┘ 艪キ�� (bug of Proolix)

		; ���珮И� �÷�М� 瓱�〓��� � ※ぅ����閧�
	mov	AX,1100h	; 粃��罔� 0 - ���珮И� ���讌���皀�赱���� 葯�籵�
			; ES:BP - �むメ ��゛�肓
	mov	BH,[f_size]	; 腮甄� 痰牀� � ��矗�罐
	mov	CX,256		; 腮甄� 瓱�〓��� � ��゛�罐
	xor	DX,DX		; ���ム �ム〓�� ��爛ぅ�錺���� 瓱�〓��
	mov	BL,DL		; ���ム ����� Л����キム�皰��
	int	10h		; EGA � VGA 痼�с ぅ��鈞 轤�� ���� ��皋↓覓

			; ��▲琥キ┘ ��｀硅...
	call	n_close		; ���琺碎 ����...
ok:	mov	AX,offset txt_ok	; 皀�痰 甌�♂キ�� �� 竅�ヨ��� ���珮И�
d_e:	JR	d_exit


n_close:
	push	[n_file]	; ���ム �����
	call	_close
	inc	sp
	inc	sp
	ret


_close:	mov	AX,close
	JR	f_proolix

_puts:	mov	AX,puts

f_proolix:		; �♂�� 皰腦� ≡ュ 瓱痰ガ�諷 �襷�〓� �襷�〓� 
	int	INT_NO
	ret	; ����, �� �ア�К�Ν� Гメ� 磴���碎 �����モ琺 甌 痰オ�
		; (�エБメ皚� 瓷��讓� �� °��)

_open:	mov	AX,open
	JR	f_proolix

_read:	mov	AX,read
	JR	f_proolix


txt_c	DB	"Font loader for EGA & VGA  Ver 0.0  (c) 1997 G.Shepelev",0
txt_err_mode	DB	"Incorrect videomode!",0
txt_derr	DB	"Disk error!",0
txt_ok	DB	"Font installed",0

mode8	DB	"08.fnt",0	; ━キ� ������ � �÷����� 葯�籵��
mode14	DB	"14.fnt",0
mode16	DB	"16.fnt",0

n_file	DW	?	; ���ム �����
f_size	DB	?	; 腮甄� 痰牀� � ��矗�罐 瓱�〓��
buff	DB	16*256 DUP (?) ; ���瓱���讚覃 ��Кム ����� Л����キム�皰��

 ENDP
	  ENDS

	END	start
