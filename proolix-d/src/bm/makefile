# OS Proolix. Boot manager makefile.
# Use Turbo C 2.0 make.exe

TasmFlags = /q /t /w2 /m4
#TasmFlags = /t /w2 /m4 /l
# LinkFlag = /c/t/d/m/s
LinkFlag = /c/d/m/s
TCOpt = -K  -wcln -wsig -wucp -wrvl -N-
# -wstv
#
TCFlag = -mt -I..\include -I..\kernel -DKERNEL -DProolix $(TCOpt)
CPPFlag = -I..\include
TC = \tc
# Proc = \usr\prool\proc
Commands = ..\command
Include = ..\include
Kernel = ..\kernel
Libs = ..\..\libs

KernelFiles = kernel.c progname.c alloc.c process.c syscall.c

ConfFiles = $(Include)\conf.c $(Include)\limits.h $(Include)\unistd.h \
$(Include)\struct.h $(Include)\prool.h $(Include)\proto.h $(Kernel)\kernel.h

.a.obj:
                tasm /ml /z $(TasmFlags) /jjumps $&.a

.c.obj:
                tcc -c $(TCFlag) -o $& $&

.asm.obj:
                tasm /ml /z $(TasmFlags) $&

.c.asm:
                tcc -c -S $(TCFlag) $&

.asm.a:
                app $&.asm $&.a

.c.i:
                cpp $(CPPFlag) $&
##############################################################################
boot.bin:       boot.com
                pklite -o boot.com boot.bin

boot.com:       boot.obj cb.obj bootl.lib $(Libs)\cs0.lib load2go.obj

                tlink /c/d/m/s/t cb boot load2go,boot,boot,bootl $(Libs)\cs0

boot.obj:       boot.c boot.h $(Include)\struct.h $(Kernel)\ppt.c
                tcc -c -B -mt -I..\include $(TCOpt) \
                -DKERNEL -DProolix -DBOOT $&
##############################################################################
bootf.bin:      bootf.com
                pklite -o bootf.com bootf.bin

bootf.com:      bootf.obj cb.obj bootl.lib $(Libs)\cs0.lib load2go.obj

                tlink /c/d/m/s/t cb bootf load2go,bootf,bootf,bootl $(Libs)\cs0

bootf.obj:      boot.c boot.h $(Include)\struct.h
                tcc -c -B -mt -I..\include $(TCOpt) -obootf \
                -DKERNEL -DProolix -DBOOT -DAUTO -DDEF_BOOT=0 boot
##############################################################################
booth.bin:      booth.com
                pklite -o booth.com booth.bin

booth.com:      booth.obj cb.obj bootl.lib $(Libs)\cs0.lib load2go.obj

                tlink /c/d/m/s/t cb booth load2go,booth,booth,bootl $(Libs)\cs0

booth.obj:      boot.c boot.h $(Include)\struct.h
                tcc -c -B -mt -I..\include $(TCOpt) -obooth \
                -DKERNEL -DProolix -DBOOT -DAUTO -DDEF_BOOT=2 boot
##############################################################################
boot.asm:       boot.c boot.h $(Include)\struct.h
                tcc -c -S -mt -I..\include $(TCOpt) \
                -DKERNEL -DProolix -DBOOT $&

a:\boot:        boot.bin
                copy boot.bin a:\boot

bootl.lib:      tlib.cmd printf.obj sec4clu2.obj mem.obj tolower.obj \
                conv.obj ctype.obj strchr.obj char_in.obj char_out.obj \
                string.obj bootread.obj out_iv.obj viewexe.obj nextclu2.obj \
                putch0.obj getch0.obj out_os.obj cursor.obj videopag.obj \
                kbhit.obj txtattr.obj int86.obj out_mbr.obj \
                hdd2.obj msdos.obj cold.obj \
                reboot.obj bootred0.obj \
                vector.obj readsec.obj \
                int13err.obj \
                cluread2.obj strlen.obj absread.obj

                tlib @ tlib.cmd

out_mbr.obj:    out_mbr.c
                tcc -c $(TCFlag) $(Kernel)\out_mbr.c

out_os.obj:     $(Kernel)\out_os.c $(ConfFiles)
                tcc -c -oout_os.obj $(TCFlag) $(Kernel)\out_os.c

strlen.obj:     $(Kernel)\strlen.c $(ConfFiles)
                tcc -c -ostrlen.obj $(TCFlag) $(Kernel)\strlen.c

absread.obj:    $(Kernel)\absread.c $(ConfFiles) $(Kernel)\kernel.h
                tcc -c -oabsread.obj $(TCFlag) $(Kernel)\absread.c

cluread2.obj:   cluread2.c $(ConfFiles) $(Kernel)\kernel.h
                tcc -c -ocluread2.obj $(TCFlag) cluread2.c

nextclu2.obj:   nextclu2.c $(ConfFiles)
                tcc -c -onextclu2.obj $(TCFlag) nextclu2.c

viewexe.obj:    $(Kernel)\viewexe.c
                tcc -c -oviewexe.obj $(TCFlag) $(Kernel)\viewexe.c

out_iv.obj:     $(Kernel)\out_iv.c $(ConfFiles) $(Kernel)\kernel.h
                tcc -c -oout_iv.obj $(TCFlag) $(Kernel)\out_iv.c

bootread.obj:   $(Kernel)\bootread.c $(ConfFiles) $(Kernel)\kernel.h
                tcc -c -obootread.obj $(TCFlag) $(Kernel)\bootread.c

string.obj:     $(Kernel)\string.c $(ConfFiles)
                tcc -c -ostring.obj $(TCFlag) $(Kernel)\string.c

char_in.obj:    $(Kernel)\char_in.c $(ConfFiles)
                tcc -c -ochar_in.obj $(TCFlag) $(Kernel)\char_in.c

char_out.obj:   $(Kernel)\char_out.c $(ConfFiles)
                tcc -c -ochar_out.obj $(TCFlag) $(Kernel)\char_out.c

strchr.obj:     $(Kernel)\strchr.c $(ConfFiles)
                tcc -c -ostrchr.obj $(TCFlag) $(Kernel)\strchr.c

ctype.obj:      $(Kernel)\ctype.c $(ConfFiles)
                tcc -c -octype.obj $(TCFlag) $(Kernel)\ctype.c

conv.obj:       $(Kernel)\conv.c $(ConfFiles)
                tcc -c -oconv.obj $(TCFlag) $(Kernel)\conv.c

printf.obj:     $(Kernel)\printf.c $(ConfFiles)
                tcc -c -B -DBOOT $(TCFlag) -oprintf.obj $(Kernel)\printf.c

printf.asm:     $(Kernel)\printf.c $(ConfFiles)
                tcc -c -S -DBOOT $(TCFlag) -oprintf.asm $(Kernel)\printf.c

#printf.obj:     printf.asm
#                tasm /ml /z $(TasmFlags) $&

sec4clu2.obj:   sec4clu2.c $(ConfFiles) $(Kernel)\kernel.h
                tcc -c $(TCFlag) sec4clu2.c

mem.obj:        $(Kernel)\mem.c $(ConfFiles)
                tcc -c $(TCFlag) -omem.obj $(Kernel)\mem.c

tolower.obj:    $(Kernel)\tolower.c $(ConfFiles)
                tcc -c $(TCFlag) -otolower.obj $(Kernel)\tolower.c

videopag.obj:   $(Kernel)\videopag.c  $(ConfFiles)
                tcc -c $(TCFlag) -ovideopag.obj $(Kernel)\videopag.c

int13err.obj:   $(Kernel)\int13err.c  $(ConfFiles)
                tcc -c $(TCFlag) -oint13err.obj $(Kernel)\int13err.c

putch0.obj:     $(Kernel)\putch0.asm
                tasm /ml /z $(TasmFlags) $(Kernel)\putch0.asm;

getch0.obj:     $(Kernel)\getch0.asm
                tasm /ml /z $(TasmFlags) $(Kernel)\getch0.asm;

kbhit.obj:      $(Kernel)\kbhit.asm
                tasm /ml /z $(TasmFlags) $(Kernel)\kbhit.asm;

txtattr.obj:    $(Kernel)\txtattr.asm
                tasm /ml /z $(TasmFlags) $(Kernel)\txtattr.asm;

hdd2.obj:       $(Kernel)\hdd2.asm
                tasm /ml /z $(TasmFlags) $(Kernel)\hdd2.asm;

msdos.obj:      $(Kernel)\msdos.asm
                tasm /ml /z $(TasmFlags) $(Kernel)\msdos.asm;

cold.obj:       $(Kernel)\cold.asm
                tasm /ml /z $(TasmFlags) $(Kernel)\cold.asm;

reboot.obj:     $(Kernel)\reboot.asm
                tasm /ml /z $(TasmFlags) $(Kernel)\reboot.asm;

bootred0.obj:   $(Kernel)\bootred0.asm
                tasm /ml /z $(TasmFlags) $(Kernel)\bootred0.asm;

vector.obj:     $(Kernel)\vector.asm
                tasm /ml /z $(TasmFlags) $(Kernel)\vector.asm;

readsec.obj:    $(Kernel)\readsec.asm
                tasm /ml /z $(TasmFlags) $(Kernel)\readsec.asm;

cursor.obj:    $(Kernel)\cursor.asm
                tasm /ml /z $(TasmFlags) $(Kernel)\cursor.asm;

int86.obj:    $(Kernel)\int86.asm
                tasm /ml /z $(TasmFlags) $(Kernel)\int86.asm;

install:        boot.bin
                copy boot.bin a:\boot

all:            boot.bin booth.bin bootf.bin

cb.obj:         cb.asm

bak:
                del *.bak
                del *.map
                del *.tmp
                del *.lst
                del *.dic
                del *.i
                del norton.ini
clean:          bak
                del *.obj
                del *.lib
                del *.bin
                del tcpick.pck
reboot:
                reboot
