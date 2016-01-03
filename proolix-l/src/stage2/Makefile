# Proolix-l stage2
# makefile for linux

LD = ld
AS = as
CPP = cpp

ASFLAG =

all:	boot ct pskr

pskr:	pskr.c proolskript.c readw.c
	$(CC) pskr.c -o pskr

boot: boot.o
	$(LD) --oformat binary -Ttext 0x0 -M -s -o $@ $< > map.lst
	chmod a-x boot

boot.o: boot.S macros.S ohw.S sayr.S saycsip.S
	sh date.sh
	$(AS) $(ASFLAG) -a=boot.lst -o $@ $<

ccc:	c1.c
	gcc c1.c -o ccc.exe
	strip ccc.exe
	mv ccc.exe ccc

CKERNELboot: kernc.o c0.o
	$(LD) $(LDFLAGS) -M -Ttext 0x0 -s -o boot c0.o kernc.o > mapc.lst
	chmod a-x boot

kernc.o:		kernc.c
	gcc -c -fno-asynchronous-unwind-tables kernc.c

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

#boot.s: boot.S macros.S
#	$(CPP) -traditional $< -o $@
clean:
	rm boot boot_ *.lst *.o ct

saycsip.s: saycsip.S macros.S
	$(CPP) -traditional $< -o $@
saycsip.o: saycsip.s
	$(AS) $(ASFLAG) -a=saycsip.lst -o $@ $<

ohw.s: ohw.S
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

cmd1: cmd1.o saycsip.o ohw.o sayr.o
#	$(LD) $(LDFLAGS) -m i386pe -M -Ttext 0x0 -s -o $@ $< saycsip.o ohw.o sayr.o> cmd1-map.lst
	$(LD) $(LDFLAGS) --oformat binary -M -Ttext 0x0 -s -o $@ $< saycsip.o ohw.o sayr.o> cmd1-map.lst
	chmod a-x cmd1
cmd1.s: cmd1.S
	$(CPP) -traditional $< -o $@
cmd1.o: cmd1.s
	$(AS) $(ASFLAG) -a=cmd1.lst -o $@ $<
cmd2:	cmd2.S
	$(AS) $(ASFLAG) -a=cmd2.lst -o cmd2.o cmd2.S
	$(LD) $(LDFLAGS) -M -Ttext 0x0 -s -o cmd2 cmd2.o > cmd2-map.lst
	chmod a-x cmd2
# cmd2com - example of binary assembler utility. compile under cygwin. run by 'run' command
cmd2com:	cmd2
	dd if=cmd2 of=cmd2com skip=1

c1s:	c1s.s
	$(AS) $(ASFLAG) -a=c1s.lst -o c1s.o c1s.s
	$(LD) $(LDFLAGS) -M -Ttext 0x0 -s -o c1s c1s.o > map.lst
	chmod a-x c1s

# cross-compile for Proolix: .c file -> asm .s file -> modified .s file -> binary
ct:	c2.c clib.c prefix.s headers.h suffix.s proolskript.c readw.c
	gcc -S -m16 c2.c
	./britva.sh c2.s
	$(AS) $(ASFLAG) -a=ctmp.lst -o ctmp.o ctmp.s
	$(LD) $(LDFLAGS) -M -Ttext 0x0 -s --oformat binary -o ct ctmp.o > map.lst
	chmod a-x ct
	ls -l ct
#	dd if=ctmp of=ct skip=4096

vexe:	vexe.c
	gcc vexe.c -o vexe.exe
vpeexe:	vpeexe.c
	gcc vpeexe.c -o vpeexe.exe
