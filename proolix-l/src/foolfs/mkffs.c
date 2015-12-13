// Fool file system v.0.2
// make empty file system

// block (sector) 0 - boot sector
// block 1 - superblock
// blocks 2 - 2+STAGE2_LEN-1 - reserved for stage2 code
// blocks 2+STAGE2_LEN - 2+STAGE2_LEN+ROOT_LEN-1 - root directory
// blocks 2+STAGE2_LEN+ROOT_LEN - end of device - blocks for files

#define STAGE2_LEN	30 // 30 sectors = 15K
#define ROOT_LEN	1
#define DISK_LEN	400 // 400 sec = 200K
#define BOOT_SECTOR	"boot.bin"
#define STAGE2		"stage2.bin"
#define DISK		"disk.bin"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void main(void)
{unsigned char buf[512];
int i, j, d, d2;
long l;

printf("Make Fool FS\n");
// create empty disk
d=open(DISK,O_CREAT|O_WRONLY);
if (d==-1) {printf("Can't open disk image\n"); return; }

// write first sector (boot sector)
for (i=0;i<512;i++) buf[i]=0;
buf[510]=0x55;
buf[511]=0xAA;
d2=open(BOOT_SECTOR,O_RDONLY);
if (d2==-1) {printf("Can't open bootsec image\n"); return; }
i=read(d2,buf,512); close(d2);
if(i<=0) {printf("Can't read bootsec image\n"); return; }
//if (i>512) {printf("Boot sector image is too long!!!!\n"); return;}
j=write(d,buf,512);
if (j!=512) {printf("Can't write block\n"); return;}

// write superblock
for (i=0;i<512;i++) buf[i]=' ';
strcpy(buf,"Fool file system v.0.2. Empty superblock...\n");
j=write(d,buf,512);
if (j!=512) {printf("Can't write block\n"); return;}

// write stage2
d2=open(STAGE2,O_RDONLY);
if (d2==-1) {printf("Can't open stage2 image\n"); return; }
while(1)
    	{
	for (i=0;i<512;i++) buf[i]='S';
	i=read(d2,buf,512);
	if(i<=0) break;
	j=write(d,buf,512);
	if (j!=512) {printf("Can't write block\n"); return;}
    	}
close(d2);

// write blocks...
for (i=0;i<512;i++) buf[i]='#';
buf[0]=0; buf[1]=0; buf[2]=0;
for (i=0;i<DISK_LEN-2-STAGE2_LEN;i++)
	{
	j=write(d,buf,512);
	if (j!=512) {printf("Can't write block. j=%i\n",j); return;}
	}
if (close(d)) {printf("Can't close\n"); return;}

}
