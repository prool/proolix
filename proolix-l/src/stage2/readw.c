int readw(int fd, char *buf, int count) // read word from file (skrypt-style)
// return code: 0 if end of file. 1 if not eof
{
int i, j; char c;

for (i=0;i<(count-1);i++)
	{
	j=read(fd,&c,1);
	if ((j==0)||(c=='\r')||(c=='\n')||(c==' ')) {*buf=0; return j;}
	if (c=='#')
		{
		while(1)
			{
			j=read(fd,&c,1);
			if (j==0) {*buf=0; return j;}
			if ((c=='\r')||(c=='\n')) break;
			}
		continue;
		}
	if (c=='!')
		{
		*buf++=c;
		for (i=1;i<(count-1);i++)
			{
			j=read(fd,&c,1);
			if ((j==0)||(c=='\r')||(c=='\n')) {*buf=0; return j;}
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
