.code16

.text

.global Buff
.global FATaddr
.global Karman

Karman:	.space		512	# buffer for 1st cluster of .EXE file
Buff:	.space		512	# buffer for 1 cluster
FATaddr:	.space	9*512# buffer for FAT. Fat size = 9 sectors
