#define BUFLEN 100

#include "headers.h"

#include "clib.c"

void help(void);

void main(void)
{
char c;
char buf [BUFLEN];

puts0("Proolix-l shell");

while (1)
	{
	puts0("\r\nEnter command -> ");
	getsn(buf,BUFLEN);
	puts0("\r\n");

	if (buf[0]==0) puts0("");
	else if (!strcmp(buf,"help")) help();
	else if (!strcmp(buf,"exit")) break;
	else if (!strcmp(buf,"quit")) break;
	else
		{
		puts0("Unknown command '");
		puts0(buf);
		puts0("'\r\n");
		}
	}

stop();
}

void help(void)
{
puts0("Proolix-l shell command\r\n\r\n\
help - help\r\n\
");
}
