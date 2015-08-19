#define BUFLEN 100

#include "headers.h"

#include "clib.c"

// global variables

void help(void);
void putch2(char c);
void putch_color(char c, char attrib);

void getpos(char *row, char *col);
void setpos (char row, char col);

#if 0
void two_parameters(int *i, int *j) // see translated text in .s file
{
*i=getch();
*j=getch();
}
#endif

void putch3(char c)
{char g_col,g_row;
char color;
color=4;
getpos(&g_row,&g_col);
switch(c)
	{
	case '\r': g_col=0; setpos(g_row,g_col); break;
	case '\n': if (g_row==24)
			scroll();
		   else
			{g_row++; setpos(g_row,g_col);}
		   break;
	default:
		putch_color(c,color);
		g_col++;
		setpos(g_row,g_col);
	}
}

void puts0(char *s)
{
    while (*s)
//	putch3(*s++);
	putch(*s++);
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
char buf [BUFLEN];
int i,j;
char c,cc;

//getpos(&c,&cc);

puts0(" Proolix-l shell. Compile ");
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
	else if (!strcmp(buf,"cls")) cls();
	else if (!strcmp(buf,"scroll")) scroll();
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
