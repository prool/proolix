# Proolix-l boot sector makefile for linux
# for install boot sector to diskette /dev/fd0 usage 'make installd'

LD = ld
AS = as
CPP = cpp

all:	boots boote patchboot analyzeboot makediskette boothdd

installvm:	all
	dd if=boothdd of=/home/prool/VirtualBox\ VMs/mach1/mach1.vhd conv=notrunc

# experimental debug boot sector
boote: boote.o patchboot analyzeboot makediskette
	$(LD) -Ttext 0x0 -s --oformat binary -o $@ $<
	chmod a-x boote

boote.o: boote.s
	$(AS) -a=boote.lst -o $@ $<

boote.s: boote.S saycsip.S
	$(CPP) -traditional $< -o $@

# boot sector for diskette with FAT12 filesystem
boots: boots.o patchboot analyzeboot makediskette
	$(LD) -Ttext 0x0 -s --oformat binary -o $@ $<
	chmod a-x boots

boots.o: boots.s
	$(AS) -a=boots.lst -o $@ $<

boots.s: boots.S
	$(CPP) -traditional -DFAT12 $< -o $@

# boot sector for diskette with FAT16 filesystem
boots16: boots16.o patchboot analyzeboot makediskette
	$(LD) -Ttext 0x0 -s --oformat binary -o $@ $<
	chmod a-x boots16

boots16.o: boots16.s
	$(AS) -a=boots16.lst -o $@ $<

boots16.s: boots.S
	$(CPP) -traditional -DFAT16 $< -o $@
# boot sector for HDD with FAT12 filesystem (experimental)
boothdd: boothdd.o patchboot analyzeboot makediskette
	$(LD) -Ttext 0x0 -s --oformat binary -o $@ $<
	chmod a-x boothdd

boothdd.o: boothdd.s
	$(AS) -a=boothdd.lst -o $@ $<

boothdd.s: boothdd.S
	$(CPP) -traditional -DFAT12 $< -o $@

#############################################################

clean:
	rm *.s *.o boots *.lst patchboot analyzeboot makediskette boot boots

patchboot:	patchboot.c

analyzeboot:	analyzeboot.c

installd: boots patchboot
	dd if=/dev/fd0 of=boot-orig count=1
	./patchboot boot-orig boots boot-result
	dd if=boot-result of=/dev/fd0 count=1
	rm -f boot-orig
	rm -f boot-result
	
