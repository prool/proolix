This is stage2 of Proolix-l

Files:

ct - Proolix-l stage2 (source - c2.c)
pscr.c - external Proolskript interpreter for Linux, Windows, etc system

---

Install

1. install boot sector (see in boot-sector directory)

2. copy binary image file 'cp' to diskette from 1 sector (see installvmfloppy in Makefile)

3. Boot from this diskette

---

For compile in 64bit Linuxes use additional library: sudo apt-get install g++-multilib

And use 32 bit flags:

main:	main.c
	gcc -o main -m32 -L/usr/lib32 main.c

---

For boot Proolix under real computer use flash drive:

1. make installflash

2. boot from USB flash stick

3. enjoy

---

Russian comments

- in oldest files in CP866 ("MSDOS alternative") codetable
- in old files in KOI8-R codetable
- in new files in UTF-8 codetable
