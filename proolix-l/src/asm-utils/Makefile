LD = ld
AS = as
CPP = cpp
CC = gcc
#CC = clang

u1: u1.o
	$(LD) --oformat binary -Ttext 0x0 -M -s -o $@ $< > map.lst
	chmod a-x $@

u1.o: u1.S macros.S sayr.S
	$(AS) $(ASFLAG) -a=u1.lst -o $@ $<

clean:
	rm *.o *.lst
