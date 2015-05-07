# Proolix-l stage2

LD = ld
AS = as
CPP = cpp

LDFLAGS = -m elf_i386
ASFLAG = --32

boot: boot.o saycsip.o ohw.o sayr.o end.o libc.o
	$(LD) $(LDFLAGS) -M -Ttext 0x0 -s --oformat binary -o $@ $< saycsip.o ohw.o sayr.o end.o libc.o > map.lst
	chmod a-x boot

libc.o:		libc.c
	gcc -m32 -c libc.c
#	$(AS) $(ASFLAG) -a=libc.lst -o libc.o libc.s

libc.s:		libc.c
	gcc -m16 -S libc.c

boot.o: boot.s
	$(AS) $(ASFLAG) -a=boot.lst -o $@ $<

boot.s: boot.S macros.S defines.h
	$(CPP) -traditional $< -o $@
clean:
	rm boot *.lst *.s *.o

saycsip.s: saycsip.S macros.S defines.h
	$(CPP) -traditional $< -o $@
saycsip.o: saycsip.s
	$(AS) $(ASFLAG) -a=saycsip.lst -o $@ $<

ohw.s: ohw.S defines.h
	$(CPP) -traditional $< -o $@
ohw.o: ohw.s
	$(AS) $(ASFLAG) -a=ohw.lst -o $@ $<

sayr.s: sayr.S
	$(CPP) -traditional $< -o $@
sayr.o: sayr.s
	$(AS) $(ASFLAG) -a=sayr.lst -o $@ $<

end.s: end.S
	$(CPP) -traditional $< -o $@
end.o: end.s
	$(AS) $(ASFLAG) -a=end.lst -o $@ $<
