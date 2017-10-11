#include "headers.h"

void subroutine(void);

void empty_function(void)
{
}

void puts0(char *s)
{
putch(*s);
s++;
putch(*s);
}

void test(char c)
{
putch(c);
}

void main (void)
{
putch('Y');
//putch('1');
//puts0("-+=");
//test('T');
//stop();
}
