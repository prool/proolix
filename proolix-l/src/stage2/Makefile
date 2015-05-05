# Proolix-l stage2
# usage 'make install' for install stage2 to diskette /dev/fd0

LD = ld
AS = as
CPP = cpp

boot: boot.o saycsip.o ohw.o sayr.o end.o
	$(LD) -M -Ttext 0x0 -s --oformat binary -o $@ $< saycsip.o ohw.o sayr.o end.o > map.lst
	chmod a-x boot

install: boot
	cp boot /mnt/floppy

boot.o: boot.s
	$(AS) -a=boot.lst -o $@ $<

boot.s: boot.S macros.S defines.h
	$(CPP) -traditional $< -o $@
clean:
	rm boot *.lst *.s *.o

saycsip.s: saycsip.S macros.S defines.h
	$(CPP) -traditional $< -o $@
saycsip.o: saycsip.s
	$(AS) -a=saycsip.lst -o $@ $<

ohw.s: ohw.S defines.h
	$(CPP) -traditional $< -o $@
ohw.o: ohw.s
	$(AS) -a=ohw.lst -o $@ $<

sayr.s: sayr.S
	$(CPP) -traditional $< -o $@
sayr.o: sayr.s
	$(AS) -a=sayr.lst -o $@ $<

end.s: end.S
	$(CPP) -traditional $< -o $@
end.o: end.s
	$(AS) -a=end.lst -o $@ $<
