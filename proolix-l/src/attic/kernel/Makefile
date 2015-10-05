# Proolix-l kernel
# makefile for linux

LD = ld
AS = as
CPP = cpp

ASFLAG =

LIBPATH = ../lib/

# cross-compile for Proolix: .c file -> asm .s file -> modified .s file -> binary
ct:	c2.c $(LIBPATH)clib.c $(LIBPATH)prefix.s $(LIBPATH)headers.h
	gcc -S -m16 -I$(LIBPATH) c2.c
	$(LIBPATH)britva.sh c2.s
	$(AS) $(ASFLAG) -a=ctmp.lst -o ctmp.o ctmp.s
	$(LD) $(LDFLAGS) -M -Ttext 0x0 -s --oformat binary -o ct ctmp.o > map.lst

clean:
	rm *.lst *.o a.out ct *.s
