# OS Proolix. Boot record makefile.
# Use Turbo C 2.0 make.exe for make Proolix kernel

TasmFlags = /i..\..\..\proc /q /t /w2 /ml /z /m3 /l
LinkFlag = /c/t/d/m/s/x
# LinkFlag = /c/t/d/x
TCFlag = -N- -K -C -mt -I..\include -DKERNEL
# TCFlag = -K -C -mt -I..\include -DKERNEL -g1 -j1
# TCFlag = -K -C -O- -G- -Z- -d- -mt -r- -u+ -v- -y-
CPPFlag = -I..\include
TC = \tc
# Proc = \usr\prool\proc
Commands = ..\command
Include = ..\include
Kernel = ..\kernel

.c.obj:
                tcc -c $(TCFlag) $&

.asm.obj:
                tasm $(TasmFlags) $&

.c.asm:
                tcc -c -S $(TCFlag) $&

.c.i:
                cpp $(CPPFlag) $&

all:            bp.com boots.bin booth12.bin booth16.bin

boots.bin:      boots.obj
                tlink $(LinkFlag) boots,boots.bin

booth12:        booth12.bin bp.com
                bp -d4 -i -y booth12.bin

booth16:        booth16.bin bp.com
                bp -d4 -i -y booth16.bin

boot:           boots.bin bp.com
                bp -d0 -i -y boots.bin

booth12.bin:    booth12.obj
                tlink $(LinkFlag) booth12,booth12.bin

booth16.bin:    booth16.obj
                tlink $(LinkFlag) booth16,booth16.bin

boots.obj:      boots.asm
booth12.obj:    booth.asm fat12.asm
                tasm $(TasmFlags) /dFAT12 booth,booth12,booth12

booth16.obj:    booth.asm fat16.asm
                tasm $(TasmFlags) /dFAT16 booth,booth16,booth16

bootrw.obj:   bootrw.asm

bp.com:         bp.c $(Include)\struct.h $(Include)\limits.h bootrw.obj \
                $(Kernel)\out_os.c $(Kernel)\out_mbr.c
                tcc -mt -lt -I\tc\include -L\tc\lib bp.c bootrw.obj

herccomp:       msherc.com msherc.bin
                comp msherc.bin msherc.com

msherc.com:     msherc.asm
                tasm $(TasmFlags) $&
                tlink /t/x msherc

boot_p.bin:     boot_p.obj
                tlink $(LinkFlag) boot_p,boot_p.bin

csipboot:       bootd.bin bp.com
                bp -da -i -y bootd.bin
bootd.bin:      bootd.obj
                tlink $(LinkFlag) bootd,bootd.bin

i install:      boot

mbr.obj:        mbr.asm
mbr.com:        mbr.obj

bootcomp:       mbr.com
                comp mbr.bin mbr.com

boot360k.2nd:   boot360k.asm
                tasm $(TasmFlag) -i\etc\sr boot360k
                tlink $(LinkFlag) boot360k,boot360k.2nd

arh arj:        pix_boot.arj
                arj f -jm pix_boot makefile
                arj f -r -t1g pix_boot *.c
                arj f -r -jm pix_boot
save:                   a:\pix_boot.arj a:\proolix.arj
a:\pix_boot.arj:        pix_boot.arj
                        copy pix_boot.arj a:\pix_boot.arj
a:\proolix.arj:         ..\proolix.arj
                        copy ..\proolix.arj a:\proolix.arj
..\proolix.arj:         pix_boot.arj
                        arj f -r -jm -xfile_id.diz ..\proolix
bak:
                del *.bak
                del *.map
                del *.tmp
                del *.lst
                del *.i
                del norton.ini

clean:          bak
                del *.obj

del delete:     clean pix_boot.arj
                arj f -jm pix_boot makefile
                arj f -r -jm -d -xmakefile pix_boot
                del tcpick.tcp
test disktest:
                arj t a:\pix_boot.arj
                arj t a:\proolix.arj
normalboot:     boot360k
                bp -r -da boot360k
reboot:
                hboot
