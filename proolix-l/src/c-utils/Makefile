# makefile for make proolix c-utilities
# cross-compilation make under OS Linux Ubuntu 16.10 64 bit!
# for crosscompilation use only 64 bit Linuses!
# gcc (Ubuntu 6.2.0-5ubuntu12) 6.2.0 20161005 or similar

c1:	c1.c prefix.S britva.sh headers.h libc.a
	gcc -m16 -S -o c1.S c1.c
	./britva.sh c1.S
	as -o c1.o -a=c1.lst ctmp.s
	ld -M -Ttext 0x0 -s --oformat binary -o c1 c1.o libc.a> c1.map
	chmod a-x c1

ctest:	ctmp.s
	as -o c1.o -a=c1.lst ctmp.s
	ld -M -Ttext 0x0 -s --oformat binary -o ctest c1.o > c1.map
	chmod a-x ctest

clean:
	rm *.o *.a c1.S *.lst *.map ctmp.s c1 ctest *.orig ctmp.sBAK ctmp.s.bak

# make .objs

sayr.o:	sayr.S
	$(AS) $(ASFLAG) -a=$@.lst -o $@ $<

saycsip.o:	saycsip.S
	$(AS) $(ASFLAG) -a=$@.lst -o $@ $<

ohw.o:	ohw.S
	$(AS) $(ASFLAG) -a=$@.lst -o $@ $<

stop.o:	stop.S
	$(AS) $(ASFLAG) -a=$@.lst -o $@ $<

syscall_test.o:	syscall_test.S
	$(AS) $(ASFLAG) -a=$@.lst -o $@ $<

putch.o:	putch.S
	$(AS) $(ASFLAG) -a=$@.lst -o $@ $<

getchar.o:	getchar.S
	$(AS) $(ASFLAG) -a=$@.lst -o $@ $<

puts0.o:	puts0.c headers.h
	gcc -m16 -S -o tmp.S $<
	./britva2.sh tmp.S
	as -o $@ -a=tmp.lst ctmp.s

puts.o:	puts.c headers.h
	gcc -m16 -S -o tmp.S $<
	./britva2.sh tmp.S
	as -o $@ -a=tmp.lst ctmp.s

libc.a:		sayr.o saycsip.o ohw.o stop.o syscall_test.o getchar.o putch.o puts0.o puts.o
	ar r libc.a $^
