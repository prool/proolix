#define BUFLEN 100

#include "headers.h"

#include "clib.c"

// global variables

char global_color;
char g_row;
char g_col;

void help(void);
void putch2(char c);
void putch_color(char c, char attrib);

#if 0
void two_parameters(int *i, int *j) // see translated text in .s file
{
*i=getch();
*j=getch();
}
#endif

void getpos(char *row, char *col)
{
*row=g_row;
*col=g_col;
}

void setpos(char row, char col)
{
g_row=row;
g_col=col;
setpos_(row,col);
}

void putch3(char c)
{char i,j;
getpos(&i,&j);
switch(c)
	{
	case '\r': j=0; setpos(i,j); break;
	case '\n': i++; setpos(i,j); break;
	default:
		putch_color(c,global_color);
		j++;
		setpos(i,j);
	}
}

void puts0(char *s)
{
    while (*s)
	putch3(*s++);
}


void test (void)
{int i,j;

setpos(0,0);

puts0("stroka 1\r\n");
puts0("stroka 2\r\n");                                                                                                       
puts0("stroka 3\r\n");

//for (i=0;i<256;i++) {global_color=i; putch3('W');}

//putch3('a');

#if 0
setpos(0,0);putch_color('1',2);
setpos(0,1);putch_color('2',3);
setpos(0,2);putch_color('3',4);

getpos(&i, &j);

puts0("\r\ngetpos ");
puthex(i);
puts0(",");
puthex(j);
puts0("\r\n");
#endif
}

void ascii(void)
{int i;
for (i=0;i<256;i++) putch(i);
}

void main(void)
{
char c;
char buf [BUFLEN];

global_color=6;
setpos(0,0);

puts0("Proolix-l shell. Compile ");
puts0(__DATE__);
puts0(" ");
puts0(__TIME__);
puts0("\r\n");

while (1)
	{
	puts0("\r\nEnter command -> ");
	getsn(buf,BUFLEN);
	puts0("\r\n");

	if (buf[0]==0) puts0("");
	else if (!strcmp(buf,"help")) help();
	else if (!strcmp(buf,"exit")) break;
	else if (!strcmp(buf,"quit")) break;
	else if (!strcmp(buf,"test")) test();
	else if (!strcmp(buf,"ascii")) ascii();
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
test - test\r\n\
help - help\r\n\
ascii - write ascii table\r\n\
exit, quit - exit\r\n\
");
}
