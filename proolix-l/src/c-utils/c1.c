// first test proolix c-language utility

#include "headers.h"

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

#if 1

arg_off=peek(6);
//puthex(arg_off);
//putch(' ');
arg_seg=peek(8);
//puthex(arg_seg);
//putch(' ');

putch('\'');
while(1)
{
c=peek_far_b(arg_seg, arg_off);
if (c==0) break;
putch(c);
arg_off++;
}
putch('\'');
#endif

#if 0
for (i=0;i<40;i++) puthex_b (peek_far_b(0,i));
#endif

puts0("\r\n");
//stop();
}
