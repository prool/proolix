// first test proolix c-language utility

#include "headers.h"

#if 0
void puts0(char *s)
{
while (*s) putch(*s++);
}
#endif

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
puts0("\r\n");
//stop();
}
