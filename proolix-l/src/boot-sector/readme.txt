This is boot sector of Proolix-l

boots.S - boot sector source
*.S - others assembler sources
boots - boot sector binary
*.c - various utilities

boots.S - classic boot sector for load file 'boot' from FAT-12 diskette
bootfdd.S - "foolish" boot sector for load binary image from raw diskette (diskette without filesystem!)

Raw diskette:

sector 0 - boot sector
from sector 1 to sector 128 binary image (maximum 64Kb) for load

boothdd.S - "foolish" boot sector for load binary image from raw hard disk

See sources! Sources rulez!

* * *

Installation of boot sector to diskette:

1.
run 'make'
and use 'makediskette' utility

2.
install stage2 to diskette (see stage2 directory)
