// C-language utility. Not work

void f1(void);

void main(void)
{
//f1();
__asm__ volatile("movb $0x0E,%ah");
__asm__ volatile("movb $0x39,%al");
__asm__ volatile("xorb %bh,%bh");
__asm__ volatile("int $0x10");
__asm__ volatile("l_stop: jmp l_stop");
}

#if 0
void f1(void)
{
__asm__ volatile("movb $0x0E,%ah");
__asm__ volatile("movb $0x39,%al");
__asm__ volatile("xorb %bh,%bh");
__asm__ volatile("int $0x10");
}
#endif
