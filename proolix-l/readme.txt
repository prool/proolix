diskette.img - Proolix-l bootable diskette (current)

proolix.iso - Proolix-l bootable CDROM (old)

Make bootable flash USB stick with 'unetbootin' utility from diskette image

-----

How to make bootable CDROM:

genisoimage -o iso.iso -b diskette.img .

(point '.' in end of line NEED! it's 'unix current path')
