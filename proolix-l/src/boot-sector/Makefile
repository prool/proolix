# Proolix-l boot sector makefile for linux
# for install boot sector to diskette /dev/fd0 usage 'make installd'

LD = ld
AS = as
CPP = cpp

VMDISK = /home/prool/VirtualBox\ VMs/proolix1/proolix1.vhd
VMFLOPPY = /home/prool/VirtualBox\ VMs/mach1/zeroes-diskette.img
FLASHDEVICE = /dev/sdb1

all:	boots patchboot analyzeboot makediskette boothdd bootfdd

installvm:	all
	dd if=boothdd of=$(VMDISK) conv=notrunc

installvmfloppy:	all
	dd if=bootfdd of=$(VMFLOPPY) conv=notrunc

installflash:	boot_f32.bin
		dd if=S(FLASHDEVICE) of=boot_f32.bin bs=1 skip=3 seek=3 count=87 conv=notrunc
		dd if=boot_f32.bin of=$(FLASHDEVICE) bs=512 count=1 conv=notrunc
		rm boot_f32.bin

boot_f32.bin:	boot_f32.asm
	    fasm boot_f32.asm

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

# master boot sector for HDD with fool filesystem
boothdd: boothdd.o patchboot analyzeboot makediskette
	$(LD) -Ttext 0x0 -s --oformat binary -o $@ $<
	chmod a-x boothdd

boothdd.o: boothdd.s
	$(AS) -a=boothdd.lst -o $@ $<

boothdd.s: boothdd.S
	$(CPP) -traditional $< -o $@

# boot sector for FDD with fool filesystem
bootfdd: bootfdd.o patchboot analyzeboot makediskette
	$(LD) -Ttext 0x0 -s --oformat binary -o $@ $<
	chmod a-x bootfdd

bootfdd.o: bootfdd.s
	$(AS) -a=bootfdd.lst -o $@ $<

bootfdd.s: bootfdd.S
	$(CPP) -traditional $< -o $@

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
	
