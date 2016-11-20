void puts0(char *s);

void main (void)
{
putch_tty('0');
putch_tty('1');
puts0("prooltest\r\n");

__asm__ volatile("int $0x20");
}

void puts0(char *s)
{
    while (*s)
	putch_tty(*s++);
}
