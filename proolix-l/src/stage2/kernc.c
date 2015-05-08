// C-language kernel (stage0)
// not work

int f2(void)
{
//__asm__ volatile("pushw %ax");
__asm__ volatile("movb $0x0E,%ah");
__asm__ volatile("movb $0x39,%al");
__asm__ volatile("xorb %bh,%bh");
__asm__ volatile("int $0x10");
//__asm__ volatile("popw %ax");

//__asm__ volatile("pushw %ax");
__asm__ volatile("xor %ah,%ah");
__asm__ volatile("int $0x16");
//__asm__ volatile("popw %ax");

return 0;
}

void c_kernel (void)
{

#if 1
//__asm__ volatile("pushw %ax");
__asm__ volatile("movb $0x0E,%ah");
__asm__ volatile("movb $0x30,%al");
__asm__ volatile("xorb %bh,%bh");
__asm__ volatile("int $0x10");
//__asm__ volatile("popw %ax");

//__asm__ volatile("pushw %ax");
__asm__ volatile("movb $0x0E,%ah");
__asm__ volatile("movb $0x31,%al");
__asm__ volatile("xorb %bh,%bh");
__asm__ volatile("int $0x10");
//__asm__ volatile("popw %ax");

//__asm__ volatile("pushw %ax");
__asm__ volatile("xor %ah,%ah");
__asm__ volatile("int $0x16");
//__asm__ volatile("popw %ax");
#endif

f2();
f2();

__asm__ volatile("movb $0x0E,%ah");
__asm__ volatile("movb $'E',%al");
__asm__ volatile("xorb %bh,%bh");
__asm__ volatile("int $0x10");
}
