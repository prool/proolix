#include "headers.h"

void subroutine(void);

void empty_function(void)
{
}

void main (void)
{
//empty_function();
//subroutine();
syscall_test();
putch('Q');
syscall_test();
putch('q');
syscall_test();
//stop();
}
