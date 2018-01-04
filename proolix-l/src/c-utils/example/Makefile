# makefile for make proolix c-utilities
# cross-compilation make under OS Linux Ubuntu 16.10 64 bit!
# for crosscompilation use only 64 bit Linuses!
# gcc (Ubuntu 6.2.0-5ubuntu12) 6.2.0 20161005 or similar

example:	example.c
	gcc -m16 -S -o asm.S $< -I..
	./britva.sh asm.S
	as -o c1.o -a=example.lst ctmp.s
	ld -M -Ttext 0x0 -s --oformat binary -o $@ c1.o ../libc.a> c1.map
	chmod a-x $@

clean:
	rm asm.S ctmp.s ctmp.orig ctmp.sBAK *.lst *.o *.map
