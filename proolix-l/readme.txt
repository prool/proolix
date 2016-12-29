Proolix-l operation system
--------------------------

diskette.img - Proolix-l bootable diskette (current)

proolix.iso - Proolix-l bootable CDROM (old)

Make bootable flash USB stick with 'unetbootin' utility from diskette image

-----

How to make bootable CDROM:

genisoimage -o iso.iso -b diskette.img .

(point '.' in end of line NEED!
it's 'unix current path')

-----

how to make empty diskette image with FAT-12 filesystem:

dd if=/dev/zero of=empty-diskette2.img bs=512 count=2880
mkfs.vfat -F 12 empty-diskette2.img
