# OS Proolix - library makefile
# Use Turbo C 2.0 make

TasmFlags = /w2 /t /q /ml /z /m4
# '-v+ -y+' is debug switches
# TCFlag = -I..\include -DProolix -v+ -y+
TCFlag = -N- -I..\include -DProolix  -wcln -wsig -wucp -wrvl -wamp -wamb -wnod \
-wpro -K
TCFlag = $(TCFlag) -g1 -j1
CPPFlag = -I..\include -DProolix
TC = \tc
Kernel = ..\kernel
Include = ..\include
Libs = ..\..\libs

!include "makefile.1"

.c.obj:
                tcc -mt -c $(TCFlag) $&

.asm.obj:
                tasm $(TasmFlags) $&

.c.asm:
                tcc -mt -c -S $(TCFlag) $&

.c.i:
                cpp $(CPPFlag) $&

all:            lib1.lib uasm cp ascii cat cat2 xonix kernel.ovl touch \
                s page videomod key ls pg vred more setcolor banner beta \
                ping help vec vmbr ce.obj p1 bp mse

lib1.lib: $(Members) tlib.cmd cpuspeed.obj $(Kernel)\syserr.obj
 del lib1.lib
 tlib lib1 /C/E -+cpuspeed -+$(Kernel)\syserr @tlib.cmd,lib1.dic

kernel.ovl:     ovl.obj c1.obj lib1.lib
 tlink /c/t/m/s/x c1 ovl,kernel.ovl,,lib1 $(Libs)\cs0 $(Kernel)\kernel

unarj:          unarj.obj decode.obj environ.obj lib1.lib
        tlink /c/t/m/s/x c1 unarj decode environ,unarj.,,lib1 $(Libs)\cs0
unarj.obj:      unarj.c   unarj.h
                tcc -mt -c $(TCFlag) -w-sig $&
environ.obj:    environ.c unarj.h
                tcc -mt -c $(TCFlag) -w-sig $&
decode.obj:     decode.c  unarj.h

cp:             cp.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
cp.obj:         cp.c

ovl.obj:        ovl.c $(Kernel)\kernel.h $(Include)\conf.c \
                $(Include)\limits.h $(Include)\unistd.h $(Include)\prool.h
                tcc -mt -c -B $(TCFlag) $&

xonix:          xonix.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
xonix.obj:      xonix.c

fork:           fork.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
fork.obj:       fork.c

ascii:          ascii.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
ascii.obj:      ascii.c

gets:           gets.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
gets.obj:       gets.c  $(Include)\conf.c

cat2:           cat2.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
cat2.obj:       cat2.c

hello:          hello.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
hello.obj:      hello.c
hello.asm:      hello.c

cat:            cat.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1 $(Libs)\cs0
cat.obj:        cat.c
cat.asm:        cat.c

touch:          touch.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
touch.obj:      touch.c

e:              e.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
e.obj:          e.c

ladder:         ladder.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
ladder.obj:     ladder.c

uasm.obj:       uasm.asm
uasm:           uasm.obj c1.obj
                tlink /c/t/m/s/x c1 $(Kernel)\regdump $(Kernel)\ohw \
                $(Kernel)\ohb $(Kernel)\sayr $&,$&.,,lib1

a:\uasm:        uasm
                copy uasm a:\uasm

cpuspeed.obj:   cpuspeed.asm UNIDEF.INC cpu_type.ash
                tasm /t/ml cpuspeed

install:        all
                mkdir a:\bin
                copy kernel.ovl a:\kernel.ovl
                copy *. a:\bin
                copy *.fnt a:\bin
                del a:\bin\makefile

installc:       all
                mkdir c:\bin
                copy kernel.ovl c:\kernel.ovl
                copy *. c:\bin
                copy *.fnt c:\bin
                del c:\bin\makefile

##############################################################################
ping:           ping.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
ping.obj:       ping.c
                tcc -mt -c -N- -I..\include -DProolix  -wcln -wsig -wucp -wrvl -wamp -wamb -wnod \
-wpro $&
#
ping.asm:       ping.c
ping.lst:       ping.asm
                tasm $(TasmFlags) /l ping.asm
                del ping.obj

##############################################################################
t:           t.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
t.obj:       t.c
t.asm:       t.c
t.lst:       t.asm
                tasm $(TasmFlags) /l t.asm
                del t.obj

##############################################################################
help:           help.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
help.obj:       help.c
help.asm:       help.c
help.lst:       help.asm
                tasm $(TasmFlags) /l help.asm
                del help.obj

##############################################################################
beta:           beta.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
beta.obj:       beta.c
beta.asm:       beta.c
beta.lst:       beta.asm
                tasm $(TasmFlags) /l beta.asm
                del beta.obj

##############################################################################
banner:         banner.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 banner,banner.,,lib1

banner.obj:     banner.c
##############################################################################
more:           more.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 more,more.,,lib1

more.obj:       more.c
##############################################################################
mse:    mse.obj c1.obj lib1.lib $(Kernel)\setjmp.obj int21.obj dosfn.obj

        tlink /c/t/m/s/x c1 int21 dosfn $(Kernel)\setjmp $(Kernel)\d2p \
        mse ,mse.,,lib1 $(Libs)\cs0

mse.obj:        mse.c mse.h
                tcc -mt -c -B $(TCFlag) $&

int21.asm:     int21.c mse.h
                tcc -mt -c -S $(TCFlag) $&

int21.obj:     int21.c
               tcc -mt -c -B $(TCFlag) $&

##############################################################################
iostone:        iostone.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 iostone,iostone.,,lib1

iostone.obj:    iostone.c
##############################################################################
setcolor:       setcolor.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 setcolor,setcolor.,,lib1

setcolor.obj:   setcolor.c
##############################################################################
vec:       vec.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 vec,vec.,,lib1

vec.obj:   vec.c
##############################################################################
m:              m.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 m,m.,,lib1
m.obj:          m.c

chess:          chess.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 chess,chess.,,lib1
chess.obj:      chess.c

vred:           vred.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 vred,vred.,,lib1
vred.obj:       vred.c

ls:             ls.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 ls,ls.,,lib1 $(Kernel)\kernel
ls.obj:         ls.c

pg:             pg.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 pg,pg.,,lib1
pg.obj:         pg.c

key:            key.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
key.obj:        key.c

videomod:       videomod.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
videomod.obj:   videomod.c

page:           page.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
page.obj:       page.c

s:              s.obj c1.obj lib1.lib
                tlink /c/t/m/s/x c1 $&,$&.,,lib1
s.obj:          s.c

a:\kernel.ovl:  kernel.ovl
                copy kernel.ovl a:\kernel.ovl

c1.obj:         c1.asm
ce.obj:         ce.asm

arh arj:        pix_kit.arj
                arj f pix_kit makefile -jm
                arj f pix_kit *.c -t1g
                arj f pix_kit -jm
save:           a:\pix_kit.arj a:\proolix.arj
a:\pix_kit.arj: pix_kit.arj
                copy pix_kit.arj a:\pix_kit.arj
a:\proolix.arj: ..\proolix.arj
                copy ..\proolix.arj a:\proolix.arj
..\proolix.arj: pix_kit.arj
                arj f ..\proolix -xfile_id.diz -xreadme.1st
clean bak:
                del *.bak
                del *.obj
                del *.map
                del *.tmp
                del *.lst
                del hello.asm
                del m.asm
                del m.obj
                del m.com
                del m.exe
                del *.dic
                del *.i
                del norton.ini
del delete:     clean pix_kit.arj
                arj f pix_kit -jm makefile
                arj f pix_kit -jm -d -xmakefile
                del tcpick.tcp
disktest:
                arj t a:\pix_kit.arj
                arj t a:\proolix.arj
reboot:
                reboot
screen:
                xonix -NoWait
##############################################################################
vmbr:       vmbr.obj c1.obj lib1.lib out_os.obj
          tlink /c/t/m/s/x c1 vmbr out_os,vmbr.,,lib1
vmbr.com: lib1.lib out_os.obj vmbr.c
          tcc -mt -lt  -N- -I\tc\include -DMSDOS \
-L\tc\lib vmbr.c ..\kernel\absread.obj out_os.obj ..\kernel\lib1.lib

vmbr.obj:   vmbr.c
                tcc -mt -c $(TCFlag) vmbr.c
c:\bin\vmbr: vmbr
           copy vmbr c:\bin\vmbr
out_os.obj:     $(Kernel)\out_os.c $(ConfFiles)
                tcc -c -oout_os.obj $(TCFlag) $(Kernel)\out_os.c
##############################################################################
bp:       bp.obj c1.obj lib1.lib
          tlink /c/t/m/s/x c1 bp ..\boot\bootrw,bp.,,lib1 $(Libs)\cs0
bp.obj:   ..\boot\bp.c
                tcc -mt -c $(TCFlag) ..\boot\bp.c
c:\bin\bp: bp
           copy bp c:\bin\bp
##############################################################################
p1:       p1.obj c1.obj lib1.lib
          tlink /c/t/m/s c1 p1 $(Kernel)\regdump $(Kernel)\ohw \
          $(Kernel)\sayr $(Kernel)\ohb,p1.,,lib1 $(Libs)\cs0
p1.obj:   p1.c
                tcc -mt -c -B $(TCFlag) p1.c
c:\bin\p1: p1
           copy p1 c:\bin\p1
##############################################################################
p2:       p2.obj c1.obj lib1.lib
          tlink /c/t/m/s/x c1 p2,p2.,,lib1 $(Libs)\cs0
p2.obj:   p2.c
                tcc -mt -c -B $(TCFlag) p2.c
c:\bin\p2: p2
           copy p2 c:\bin\p2
##############################################################################
p3:       p3.obj c1.obj lib1.lib
          tlink /c/t/m/s/x c1 p3,p3.,,lib1 $(Libs)\cs0
p3.obj:   p3.c
                tcc -mt -c -B $(TCFlag) p3.c
c:\bin\p3: p3
           copy p3 c:\bin\p3
##############################################################################

