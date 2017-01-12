#!/bin/sh
cd ../..
echo Make bootable Proolix CD ISO image from Proolix bootable diskette image
rm proolix.iso
genisoimage -o proolix.iso -b diskette.img .
