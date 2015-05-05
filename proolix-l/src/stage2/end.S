# Это конец, подумал Штирлиц

.code16

.text

.global Buff
.global FATaddr

Buff:	.byte	# buffer for 1 cluster

FATaddr	=	Buff+512*1	# buffer for FAT
				# в данном случае размер FAT - 9 секторов
