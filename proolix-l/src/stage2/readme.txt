This is stage2 of Proolix-l

Files:

boot.S (in this directory!) is stage2 source
boot - binary executable
cmd1 - example of binary command, compiled under Linux (see Makefile.linux)
cmd2com - example of binary command, compiled under cygwin (see Makefile.cygwin)
ct - Proolix-l shell (sources - c2.c)
pscr.c - external Proolskript interpreter for Linux, Windows, etc system

---

Install

1. install boot sector (see in boot-sector directory)

2. copy files 'boot' and 'cp' to diskette (file must be FIRST on diskette!)

3. Boot from this diskette

---

How to install boot to image of diskette:

sudo mount diskette /mnt/fdd
cp boot /mnt/fdd
cp ct /mnt/fdd
sudo umount /mnt/fdd

---

For compile in 64bit Linuxes: sudo apt-get install g++-multilib

And use 32 bit flags:

main:	main.c
	gcc -o main -m32 -L/usr/lib32 main.c

---

For boot Proolix under real computer use flash drive:

1. install proolix diskette image to flash drive uses Unetbootin (http://unetbootin.sourceforge.net/)

2. boot from USB flash stick

3. enjoy

---

Russian comments

- in oldest files in CP866 ("MSDOS alternative") codetable
- in old files in KOI8-R codetable
- in new files in UTF-8 codetable
