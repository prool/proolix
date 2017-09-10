#define peek(ADR) 0
#define peek2(SEG,OFFSET) 0

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

