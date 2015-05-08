# Это конец, подумал Штирлиц

.code16

.text

.global Buff
.global FATaddr

Buff:	.space		512	# buffer for 1 cluster

FATaddr:	.space	9*512# buffer for FAT
				# в данном случае размер FAT - 9 секторов
