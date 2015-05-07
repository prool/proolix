int putchar(int c)
{
}

int puts(char *str)
{
while(*str++)
    {
    putchar(*str);
    }
}

void f1 (void)
{
__asm__ volatile("pushw %ax");
__asm__ volatile("movw $0x0E33,%ax");
__asm__ volatile("int $0x10");
__asm__ volatile("popw %ax");
//puts("test\n");
}

void f2 (void)
{
f1();
}