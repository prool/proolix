# OS Proolix. Kernel makefile.
# Use Turbo C 2.0 make.exe for make Proolix kernel

TasmFlags = /q /t /w2 /m4
#TasmFlags = /t /w2 /m4 /l
# LinkFlag = /c/t/d/m/s
LinkFlag = /c/d/m/s
TCOpt = -K -C -wcln -wsig -wucp -wrvl -wamp -wamb -wnod -wpro -j1 -g1
# -wstv
#
TCFlag = -N- -ms -I..\include -DKERNEL -DProolix $(TCOpt)
CPPFlag = -I..\include
TC = \tc
# Proc = \usr\prool\proc
Commands = ..\command
Include = ..\include
Libs = ..\..\libs

KernelFiles = kernel.c alloc.c process.c syscall.c

ConfFiles = $(Include)\conf.c $(Include)\limits.h $(Include)\unistd.h \
$(Include)\struct.h $(Include)\prool.h $(Include)\proto.h kernel.h \
$(Include)\stdio.h

kernel.bin:     kernel.exe
                pklite -o kernel.exe kernel.bin

kernel.exe:     c0.obj kernel.obj kernel.lib
                tlink $(LinkFlag) c0 kernel,kernel,kernel,kernel $(Libs)\cs0

#kernel.obj:     kernel.asm
#                tasm /ml /z $(TasmFlags) $&

#kernel.bin:     app.com c0.obj kernel.obj kernel.lib
#                tlink $(LinkFlag) c0 kernel,kernel.bin,kernel,kernel $(Libs)\cs0
#                dir kernel.bin
#               tcc -lt -L. $(TCFlag) -ekernel.bin kernel.c $(Libs)\cs0.lib kernel.lib

boot:           boot.obj cb.obj kernel.lib
                tlink /c/d/m/s/t cb boot,boot.,boot,kernel $(Libs)\cs0
boot.obj:       boot.c
                tcc -c -B -mt -I..\include $(TCOpt) \
                -DKERNEL -DProolix -DBOOT $&
boot.asm:       boot.c
                tcc -c -S -mt -I..\include $(TCOpt) \
                -DKERNEL -DProolix -DBOOT $&
a:\boot:        boot
                copy boot a:\boot

printf.obj:     printf.c $(ConfFiles) $(Include)\stdarg.h
                tcc -c -B $(TCFlag) $&

#printf.obj:     printf.asm
#                tasm /ml /z $(TasmFlags) $&

printf.asm:     printf.c
                tcc -c -S $(TCFlag) $&


#kernel.obj:     kernel.c $(KernelFiles) $(ConfFiles) $(Include)\sys\types.h
#                tcc -c -B $(TCFlag) $&

kernel.obj:     kernel.c $(KernelFiles) $(ConfFiles) $(Include)\sys\types.h
                tcc -j -g -c -B $(TCFlag) $&

kernel.lib:     tlib.cmd k2.obj io.obj minus.obj tr.obj videopag.obj \
                ints.obj int0.obj ints5.obj int5dump.obj int8.obj int1b.obj \
                int13err.obj int86.obj strtoul.obj service.obj exitw.obj \
                nulldev.obj stream.obj gets.obj printf.obj \
                fprintf.obj setjmp.obj \
                char_in.obj char_out.obj qsort.obj readsec2.obj \
                time.obj sh.obj memd.obj stat.obj \
                out_os.obj out_mbr.obj out_boot.obj out_iv.obj out_dev.obj \
                vec.obj key.obj cmos.obj nocmos.obj outbot32.obj \
                absread.obj abswrite.obj bootread.obj \
                bootred0.obj cluread.obj sec4clu.obj \
                nextclu.obj \
                regdump.obj sayr.obj ohw.obj ohb.obj clrscr0.obj clrscr.obj \
                readsec.obj \
                cursor.obj reboot.obj cold.obj hdd2.obj genint.obj msdos.obj \
                loop.obj run_boot.obj \
                comport.obj port.obj vector.obj initv.obj kbhit.obj end.obj \
                getch0.obj putch0.obj putch1.obj putch2.obj putch3.obj \
                txtattr.obj intercept.obj strchr.obj strdup.obj string.obj \
                strlen.obj mem.obj memmove.obj \
                stdlib.obj tolower.obj ctype.obj conv.obj syserr.obj ls.obj \
                rm.obj mv.obj cp.obj cat.obj de.obj e.obj \
                stdio.obj mouse.obj msherc.obj msherc1.obj dir.obj \
                debug.obj p2d.obj d2p.obj viewexe.obj rand.obj stimer.obj \
                putchp.obj \

                tlib @ tlib.cmd

gentbl.com:     gentbl.c $(Include)\conf.c
                tcc -mt -lt -v- -y- -I\tc\include -L\tc\lib gentbl
                del gentbl.obj
                del gentbl.exe

app.com:        app.c
                tcc -mt -lt -v- -y- app
                del app.obj
                del app.exe

vexe.com:       vexe.c viewexe.c
                tcc $(TCOpt) -mt -lt -v- -y- vexe.c

vexe.asm:       vexe.c
                tcc -c -S $(TCOpt) -mt -lt -v- -y- vexe.c

vexe.i:         vexe.c
                cpp vexe.c

ls.obj:         ls.c $(ConfFiles)
a:\ls.c:        ls.c
                copy ls.c a:\ls.c

gets.obj:       gets.c gets.inc $(ConfFiles)

stream.obj:     stream.c gets.inc fgets.inc $(ConfFiles)
#stream.obj:     stream.a debug.mac
stream.asm:     stream.c gets.inc fgets.inc $(ConfFiles)
stream.a:       stream.asm

stdio.obj:      stdio.c $(ConfFiles)
#stdio.obj:      stdio.a debug.mac
stdio.asm:      stdio.c $(ConfFiles)
stdio.a:        stdio.asm

io.obj:         io.c $(ConfFiles)
#io.obj:         io.a debug.mac
io.asm:         io.c $(ConfFiles)
io.a:           io.asm

dir.obj:        dir.c $(ConfFiles) $(Include)\dirent.h
#dir.obj:        dir.a debug.mac
dir.asm:        dir.c $(ConfFiles)
dir.a:          dir.asm

sched.asm:      sched.c $(ConfFiles)
                tcc -c -S $(TCFlag) $&

sched.obj:      sched.asm
                tasm /ml /z $(TasmFlags) $&

int8.obj:       int8.c $(ConfFiles)
                tcc -c -B $(TCFlag) $&

int8.asm:       int8.c $(ConfFiles)
                tcc -c -S $(TCFlag) $&

sh.obj:         sh.c $(ConfFiles)
                tcc -c -B $(TCFlag) $&

int1b.obj:      int1b.c $(ConfFiles)
                tcc -c -B $(TCFlag) $&

viewexe.obj:    viewexe.c
p2d.obj:        p2d.c $(ConfFiles)
d2p.obj:        d2p.c
debug.obj:      debug.asm
int20.obj:      int20.asm
reboot.obj:     reboot.asm
cold.obj:       cold.asm
msdos.obj:      msdos.asm
run_boot.obj:   run_boot.asm
hdd2.obj:       hdd2.asm
clrscr0.obj:    clrscr0.asm
readsec.obj:    readsec.asm
bootred0.obj:   bootred0.asm
exitw.obj:      exitw.asm
putch0.obj:     putch0.asm
putch1.obj:     putch1.asm
putch2.obj:     putch2.asm
putch3.obj:     putch3.c $(ConfFiles)
int86.obj:      int86.asm
msherc.obj:     msherc.asm
msherc1.obj:    msherc1.asm
mouse.obj:      mouse.asm
syserr.obj:     syserr.c
users.obj:      users.c $(ConfFiles)
qsort.obj:      qsort.c $(ConfFiles)
dos.obj:        dos.c $(ConfFiles) $(Include)\dos.h
fprintf.obj:    fprintf.c $(ConfFiles)
char_in.obj:    char_in.c $(ConfFiles)
char_out.obj:   char_out.c $(ConfFiles)
strchr.obj:     strchr.c $(ConfFiles)
strdup.obj:     strdup.c $(ConfFiles)
strlen.obj:     strlen.c $(ConfFiles)
string.obj:     string.c $(ConfFiles)
strtoul.obj:    strtoul.c $(ConfFiles)
mem.obj:        mem.c $(ConfFiles)
memmove.obj:    memmove.c $(ConfFiles)
stdlib.obj:     stdlib.c $(ConfFiles)
tolower.obj:    tolower.c $(ConfFiles)
ctype.obj:      ctype.c $(ConfFiles)
conv.obj:       conv.c $(ConfFiles)
rm.obj:         rm.c $(ConfFiles)
mv.obj:         mv.c $(ConfFiles)
cp.obj:         cp.c $(ConfFiles)
cat.obj:        cat.c $(ConfFiles)
de.obj:         de.c $(ConfFiles)
e.obj:          e.c $(ConfFiles)
timer.obj:      timer.c $(ConfFiles)
memd.obj:       memd.c $(ConfFiles)
vec.obj:        vec.c $(ConfFiles)
out_iv.obj:     out_iv.c $(ConfFiles)
msh.obj:        msh.c $(ConfFiles) msh.h
stat.obj:       stat.c $(ConfFiles) $(Include)\stdio.h \
$(Include)\struct.h $(Include)\sys\types.h $(Include)\sys\stat.h
cluread.obj:    cluread.c $(ConfFiles)
nextclu.obj:    nextclu.c $(ConfFiles)
sec4clu.obj:    sec4clu.c $(ConfFiles)
out_mbr.obj:    out_mbr.c $(ConfFiles)
out_dev.obj:    out_dev.c $(ConfFiles)
out_os.obj:     out_os.c $(ConfFiles)
videopag.obj:   videopag.c $(ConfFiles)
absread.obj:    absread.c $(ConfFiles)
abswrite.obj:   abswrite.c $(ConfFiles)
tr.obj:         tr.c $(ConfFiles)
k2.obj:         k2.c ppt.c $(ConfFiles)
ints.obj:       ints.c $(ConfFiles)
int0.obj:       int0.c $(ConfFiles)
ints5.obj:      ints5.c $(ConfFiles)
minus.obj:      minus.c $(ConfFiles)
nulldev.obj:    nulldev.c $(ConfFiles)
cpm.obj:        cpm.c $(ConfFiles)
bootread.obj:   bootread.c $(ConfFiles)
readsec2.obj:   readsec2.c $(ConfFiles)
int13err.obj:   int13err.c $(ConfFiles)
bios.obj:       bios.c $(ConfFiles) bios.h
time.obj:       time.c $(ConfFiles)
stimer.obj:     stimer.c $(ConfFiles)
regdump.obj:    regdump.asm macros.asm
int5dump.obj:   int5dump.asm
sayr.obj:       sayr.asm
setjmp.obj:     setjmp.asm
dosfn.obj:      dosfn.c $(ConfFiles)
out_boot.obj:   out_boot.c $(ConfFiles)
outbot32.obj:   outbot32.c $(ConfFiles)
putchp.obj:     putchp.c $(ConfFiles)
clrscr.obj:     clrscr.c $(ConfFiles)

a:\kernel:      kernel.bin
                copy kernel.bin a:\kernel

d:\kernel.exe:  kernel.bin
                copy kernel.bin d:\kernel

install installa:        a:\kernel

c:\kernel.exe:  kernel.bin
                copy kernel.bin c:\kernel.exe

installc:       c:\kernel.exe

installac:      installa installc

m.com:  m.obj
                tlink $(LinkFlag) c0 m,m,m,$(TC)\lib\cs
                del m.exe
m.obj:          m.c
m.asm:          m.c
syskern.asm gentbl:     syscall.tbl gentbl.com
                        gentbl $(Commands) -n
c0.obj:         c0.asm syskern.asm

cb.obj:         cb.asm

arh arj:        pix_kern.arj
                arj f -jm pix_kern makefile
                arj f -r -t1g pix_kern *.c *.h
                arj f -r -jm pix_kern
save:                   a:\pix_kern.arj a:\proolix.arj
a:\pix_kern.arj:        pix_kern.arj
                        copy pix_kern.arj a:\pix_kern.arj
a:\proolix.arj:         ..\proolix.arj
                        copy ..\proolix.arj a:\proolix.arj
..\proolix.arj:         pix_kern.arj
                        arj f -r -jm -xfile_id.diz ..\proolix
bak:            del_a
                del *.bak
                del *.map
                del *.tmp
                del *.lst
                del kernel.asm
                del *.dic
                del *.i
                del norton.ini
clean:          bak
                del *.obj
                del *.bin
                del *.com
                del kernel.exe
                del kernel.lib
                del tcpick.pck
                del aaa
del_a:
                del *.a

del delete:     clean pix_kern.arj
                arj f -jm pix_kern makefile
                arj f -r -jm -d -xmakefile pix_kern
                del tcpick.tcp
test disktest:
                arj t a:\pix_kern.arj
                arj t a:\proolix.arj
reboot:
                reboot
.a.obj:
                tasm /ml /z $(TasmFlags) /jjumps $&.a

.c.obj:
                tcc -c $(TCFlag) $&

.asm.obj:
                tasm /ml /z $(TasmFlags) $&

.c.asm:
                tcc -c -S $(TCFlag) $&

.asm.a:
                app $&.asm $&.a

.c.i:
                cpp $(CPPFlag) $&
