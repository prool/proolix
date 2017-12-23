# makefile for make proolix fortran utilities
# cross-compilation make under OS Linux Ubuntu 16.10 x86_64
# for crosscompilation use only 64 bit Linuses!

all:	f1

f1.S:	f1.f
	gfortran -m16 f1.f -o f1.S -S
f1.o:	f1.S prefix.S
	./britva.sh f1.S
	as -o f1.o -a=f1.lst ftmp.s
f1:	f1.o
	ld -M -Ttext 0x0 -s --oformat binary -o f1 f1.o > f1.map

clean:
	rm f1.S ftmp.s f1.o f1
