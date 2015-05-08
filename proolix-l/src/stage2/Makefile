# Proolix-l stage2

LD = ld
AS = as
CPP = cpp

LDFLAGS = -m elf_i386
ASFLAG = --32

boot: boot.o saycsip.o ohw.o sayr.o end.o libc.o lib2.o
	$(LD) $(LDFLAGS) -M -Ttext 0x0 -s --oformat binary -o $@ $< saycsip.o ohw.o sayr.o end.o > map.lst
	chmod a-x boot

CKERNELboot: kernc.o c0.o
	$(LD) $(LDFLAGS) -M -Ttext 0x0 -s --oformat binary -o boot c0.o kernc.o > mapc.lst
	chmod a-x boot

kernc.o:		kernc.c
	gcc -m32 -c -fno-asynchronous-unwind-tables kernc.c

kernc.s:	kernc.c
	gcc -m32 -S -fno-asynchronous-unwind-tables kernc.c

libc.o:		libc.c
	gcc -m32 -c libc.c
#	$(AS) $(ASFLAG) -a=libc.lst -o libc.o libc.s

lib2.o:		lib2.c
	gcc -m32 -c lib2.c

libc.s:		libc.c
	gcc -m16 -S libc.c

lib2.s:		lib2.c
	gcc -m16 -S lib2.c

boot.o: boot.s
	$(AS) $(ASFLAG) -a=boot.lst -o $@ $<

boot.s: boot.S macros.S defines.h
	$(CPP) -traditional $< -o $@
clean:
	rm boot kernc *.lst *.s *.o a.out

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

c0.s: c0.S
	$(CPP) -traditional $< -o $@
c0.o: c0.s
	$(AS) $(ASFLAG) -a=c0.lst -o $@ $<
