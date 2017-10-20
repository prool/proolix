// test proolix c-language utility

#include "headers.h"

void main (void)
{
short int i;

puts0("c2 test pgm\r\n");
i=open("c1", O_READ);
puts0("open ret code="); puthex(i);
puts0("\r\n");
}
