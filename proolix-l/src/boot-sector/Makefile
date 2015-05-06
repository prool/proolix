# Proolix-l boot sector makefile
# for install boot sector to diskette /dev/fd0 usage 'make install'

LD = ld
AS = as
CPP = cpp

boots: boots.o patchboot patchboot analyzeboot makediskette stage2
	$(LD) -Ttext 0x0 -s --oformat binary -o $@ $<
	chmod a-x boots
	./makediskette e.img boots stage2

stage2: stage2.o
	$(LD) -M -Ttext 0x0 -s --oformat binary -o $@ $< > map.lst
	chmod a-x stage2

stage2.o: stage2.s
	$(AS) -a=stage2.lst -o $@ $<

stage2.s: stage2.S macros.S
	$(CPP) -traditional $< -o $@

boots.o: boots.s
	$(AS) -a=boots.lst -o $@ $<

boots.s: boots.S
	$(CPP) -traditional $< -o $@

clean:
	rm *.s *.o boots *.lst patchboot analyzeboot stage2

patchboot:	patchboot.c

analyzeboot:	analyzeboot.c

makediskette:	makediskette.c

install: boots patchboot
	dd if=/dev/fd0 of=boot-orig count=1
	./patchboot boot-orig boots boot-result
	dd if=boot-result of=/dev/fd0 count=1
	rm -f boot-orig
	rm -f boot-result
	
