// C-language subroutine
// not work

void f2 (void)
{
#if 1
__asm__ volatile("pushw %ax");
__asm__ volatile("movb $0x0E,%ah");
__asm__ volatile("movb $0x32,%al");
__asm__ volatile("int $0x10");
__asm__ volatile("popw %ax");
#endif

}
int putchar(int c)
{
#if 0
__asm__ volatile("pushw %ax");
__asm__ volatile("movb $0x0E,%ah");
__asm__ volatile("movb $0x39,%al");
__asm__ volatile("int $0x10");
__asm__ volatile("popw %ax");
#endif
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
#if 1
__asm__ volatile("pushw %ax");
__asm__ volatile("movb $0x0E,%ah");
__asm__ volatile("movb $0x31,%al");
__asm__ volatile("int $0x10");
__asm__ volatile("popw %ax");
#endif
f2();
#if 1
__asm__ volatile("pushw %ax");
__asm__ volatile("movb $0x0E,%ah");
__asm__ volatile("movb $0x33,%al");
__asm__ volatile("int $0x10");
__asm__ volatile("popw %ax");
#endif
//puts("test\n");
}
