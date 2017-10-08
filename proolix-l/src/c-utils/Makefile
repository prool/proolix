c1:	c1.c prefix.S britva.sh
	gcc -m16 -S -o c1.S c1.c
	./britva.sh c1.S
	as -o c1.o -a=c1.lst ctmp.s
	ld -M -Ttext 0x0 -s --oformat binary -o c1 c1.o > c1.map
	chmod a-x c1

ctest:
	as -o c1.o -a=c1.lst ctmp.s
	ld -M -Ttext 0x0 -s --oformat binary -o ctest c1.o > c1.map

clean:
	rm *.o c1.S *.lst *.map ctmp.s c1 ctest
