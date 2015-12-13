#!/bin/sh
echo Patch of HDD image for Virtual Box
dd if=/home/prool/PROJECTS/proolix/proolix-l/src/boot-sector/boote of=proolix2.vhd conv=notrunc
