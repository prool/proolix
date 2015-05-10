LD = ld
AS = as
CPP = cpp

LDFLAGS = -m elf_i386
ASFLAG = --32

cmdc: cmdc.o
	$(LD) $(LDFLAGS) -M -Ttext 0x0 -s --oformat binary -o $@ $< > map.lst
	chmod a-x cmdc

#cmdc:	cmdc.c
#	gcc -o cmdc -m32 cmdc.c

cmdc.s:	cmdc.c
	gcc -S -m32 cmdc.c

cmdc.o: cmdc.s
	$(AS) $(ASFLAG) -a=cmdc.lst -o $@ $<
