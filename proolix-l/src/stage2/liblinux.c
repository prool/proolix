short int peek2(short int seg, short int offset)
{
int ram;
int i;

ram=open("/dev/mem", O_RDONLY);
if (ram==-1) {printf("Can't open RAM. Use root privileges!\n"); return 0;}
if (lseek(ram, offset, SEEK_SET)==-1) {printf("Can'r seek RAM\n"); return 0;}
i=0;
if (read(ram, &i, 2)!=2) {printf("Can't read RAM\n"); return 0;}
close(ram);
return i;
}

unsigned char peek3(unsigned short int seg, unsigned short int offset)
{
return peek2(seg,offset);
}

int peek (int addr)
{
int ram;
int i;

ram=open("/dev/mem", O_RDONLY);
if (ram==-1) {printf("Can't open RAM. Use root privileges!\n"); return 0;}
if (lseek(ram, addr, SEEK_SET)==-1) {printf("Can'r seek RAM\n"); return 0;}
i=0;
if (read(ram, &i, 2)!=2) {printf("Can't read RAM\n"); return 0;}
close(ram);
return i;
}

void poke(char value, short int seg, short int offset)
{
}

void putch_color (char c, char attrib)
{
//printf("debug putch_color() char %i\n",c);
putchar(c);
}

int reads(int fd, char *buf, int count) // read string from file
{
int i, j; char c;

for (i=0;i<(count-1);i++)
	{
	j=read(fd,&c,1);
	if ((j==0)||(c=='\r')||(c=='\n')) {*buf=0; return 1;}
	*buf++=c;
	}
*buf=0;
return 0;
}

