How to install Proolix-l to physical diskette
---------------------------------------------

0. Format diskette as FAT-12 filesystem

1. Install boot sector to diskette (see readme.txt in 'boot-sector' directory)

2. Make and copy files 'boot' and 'ct' to diskette (see readme.txt file in 'stage2' directory)

3. Boot physical machine from diskette

4. Enjoy

How to install Proolix-l to diskette image
------------------------------------------

0. Make empty diskette image (file)

dd if=/dev/zero of=diskette.img bs=512 count=2880
mkfs.vfat -F 12 diskette.img

1. Install boot sector to diskette image (see readme.txt in 'boot-sector' directory)

2. Make and copy files 'boot' and 'ct' to diskette image (see readme.txt file in 'stage2' directory)

 or use prepared diskette image (file 'diskette.img')

3. Boot from diskette image (for example, use virtual machine in VirtualBox)

4. Enjoy

How to install Proolix-l to CDROM

1. Install Proolix to diskette image

2. Make bootable CDROM ISO image from diskette image

- or use prepared ISO image (file 'proolix.iso')

3. Boot virtual machine from CDROM image 

or burn CDROM image and boot physical machine from CDROM

4. Enjoy
