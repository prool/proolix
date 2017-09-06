int readw(int fd, char *buf, int count) // read word from file (skrypt-style)
// return code: 0 if end of file. 1 if not eof
{
int i, j; char c;

for (i=0;i<(count-1);i++)
	{
	j=readc(fd,&c);
	if ((j==-1)||(c=='\r')||(c=='\n')||(c==' ')) {*buf=0; return 0;}
	if (c=='#')
		{
		while(1)
			{
			j=readc(fd,&c);
			if (j==-1) {*buf=0; return 0;}
			if ((c=='\r')||(c=='\n')) break;
			}
		continue;
		}
	if (c=='!')
		{
		*buf++=c;
		for (i=1;i<(count-1);i++)
			{
			j=readc(fd,&c);
			if ((j==-1)||(c=='\r')||(c=='\n')) {*buf=0; return 0;}
			*buf++=c;
			}
		*buf=0;
		return 1;
		}
	*buf++=c;
	}
*buf=0;
return 1;
}
