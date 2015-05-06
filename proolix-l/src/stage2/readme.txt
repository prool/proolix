This is stage2 of Proolix-l

boot.S (in this directory!) is stage2 source
boot - binary

Install

1. install boot sector (see in boot-sector directory)

2. copy file 'boot' to diskette (file must be FIRST on diskette!)

3. Boot from this diskette

How to install boot to image of diskette:

sudo mount diskette /mnt/fdd
cp boots /mnt/fdd
sudo umount /mnt/fdd
