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
short int i;

unsigned short int arg_off, arg_seg;
#if 0
syscall_test();
putch(':');
puts("press anykey");
c=getchar();
puts0("\r\n");
puts0("You pressed ");
putch(c);
#endif

#if 0
arg_off=peek(6);
puthex(arg_off);
arg_seg=peek(8);
puthex(arg_seg);

c=peek_far_b(arg_seg, arg_off);
putch(c);
#endif

for (i=0;i<40;i++) puthex_b (peek_far_b(0,i));

puts0("\r\n");
//stop();
}
