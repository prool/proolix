// system function getsn

#include "headers.h"

char *getsn(char *str, int len)
{char c; int i;

#ifdef DEBUG
char *str0;
str0=str;
#endif

i=0;
while(1)
	{
	c=getchar(); putch(c);
	if (c==0x0DU) {*str=0; break;}
#ifdef PEMU
	if (c==0x0AU) {*str=0; break;}
#endif
	if (c==8) {if (i>0) {i--;str--;}}
	else 
		{*str=c;
		if (++i>=(len-1)) {*str=0; break;}
		str++;
		}
	}

#ifdef DEBUG
puts0("debug getsn() ");
while (*str0) puthex_b(*str0++);
#endif
puts0("\r\n");

return str;
}
