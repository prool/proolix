# OS Proolix - development kit makefile
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

.obj.com:
        tlink /c/t/m/s/x ..\command\c1 $&,$&,,..\command\lib1 $(Libs)\cs0

.c.obj:
                tcc -ms -c $(TCFlag) $&

.asm.obj:
                tasm $(TasmFlags) $&

.c.asm:
                tcc -mt -c -S $(TCFlag) $&

.c.i:
                cpp $(CPPFlag) $&

all:            hello he bpatch.com hc

bpatch.com:     bpatch.c
                tcc -mt -lt -I\tc\include -L\tc\lib bpatch
##############################################################################
#               memory model small
he:             he.obj
                tlink /c/m/s/x ..\command\ce $&,$&.,,..\command\lib1 $(Libs)\cs0
                bpatch he 0xAFDE
he.obj:         he.c
                tcc -ms -c $(TCFlag) $&
he.asm:         he.c
                tcc -ms -c -S $(TCFlag) $&
##############################################################################
#               memory model small
hc:             hc.obj
                tlink /c/m/s/x ..\command\ce $&,$&.,,..\command\lib1 $(Libs)\cc0
                bpatch hc 0xAFDE
hc.obj:         hc.c
                tcc -mc -c $(TCFlag) $&
hc.asm:         hc.c
                tcc -mc -c -S $(TCFlag) $&
##############################################################################
hello:          hello.obj
                tlink /c/t/m/s/x ..\command\c1 $&,$&.,,..\command\lib1
hello.obj:      hello.c
hello.asm:      hello.c

install:        all
                copy hello a:\bin\hello
                copy he a:\bin\he
                copy hc a:\bin\hc

clean:
                del *.bak
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

sh:             sh.obj
                tlink /c/t/m/s/x ..\command\c1 $&,$&.,,..\command\lib1
sh.obj:         sh.c zadroka.h $(Include)\conf.c $(Include)\limits.h \
                $(Include)\unistd.h $(Include)\struct.h
c:\bin\sh:      sh
                copy sh c:\bin\sh

t1:             t1.obj
                tlink /c/t/m/s/x ..\command\c1 $&,$&.,,..\command\lib1
t1.obj:         t1.c

t2:             t2.obj
                tlink /c/t/m/s/x ..\command\c1 $&,$&.,,..\command\lib1
t2.obj:         t2.c


