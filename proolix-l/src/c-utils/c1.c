// first test proolix c-language utility

#include "headers.h"

void puts0(char *s)
{
while (*s) putch(*s++);
}

void test(char c)
{
putch(c);
}

void main (void)
{char c;
syscall_test();
putch(':');
puts0("press anykey\r\n");
c=getchar();
#if 1
puts0("\r\n");
puts0("You pressed ");
putch(c);
#endif
//stop();
puts0("\r\n");
//test('t');
//empty_function();
//stop();
}
