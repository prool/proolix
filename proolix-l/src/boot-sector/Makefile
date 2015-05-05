# Proolix-l boot sector makefile
# for install boot sector to diskette /dev/fd0 usage 'make install'

LD = ld
AS = as
CPP = cpp

boots: boots.o patchboot
	$(LD) -Ttext 0x0 -s --oformat binary -o $@ $<
	chmod a-x boots

boots.o: boots.s
	$(AS) -a=boots.lst -o $@ $<

boots.s: boots.S
	$(CPP) -traditional $< -o $@

clean:
	rm boots.s boots.o boots boots.lst patchboot analyzeboot

patchboot:	patchboot.c

analyzeboot:	analyzeboot.c

install: boots patchboot
	dd if=/dev/fd0 of=boot-orig count=1
	./patchboot boot-orig boots boot-result
	dd if=boot-result of=/dev/fd0 count=1
	rm -f boot-orig
	rm -f boot-result
	