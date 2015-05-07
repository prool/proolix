This is stage2 of Proolix-l

boot.S (in this directory!) is stage2 source
boot - binary

---

Install

1. install boot sector (see in boot-sector directory)

2. copy file 'boot' to diskette (file must be FIRST on diskette!)

3. Boot from this diskette

---

How to install boot to image of diskette:

sudo mount diskette /mnt/fdd
cp boots /mnt/fdd
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

