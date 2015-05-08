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
