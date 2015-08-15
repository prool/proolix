void stop(void);
void putch(char c);
void puts0(char *s);

void getch(void)
{
__asm__ volatile("xor %ah,%ah");
__asm__ volatile("int $0x16");
}

void main(void)
{
char c;

puts0("C utility \r\n");

puts0("Press any key");

getch();

stop();
}

void puts0(char *s)
{
/*
switch(((int)s)%16)
	{
	case 0: putch('0');break;
	case 1: putch('1');break;
	case 2: putch('2');break;
	case 3: putch('3');break;
	case 4: putch('4');break;
	case 5: putch('5');break;
	case 6: putch('6');break;
	case 7: putch('7');break;
	case 8: putch('8');break;
	case 9: putch('9');break;
	case 10: putch('A');break;
	case 11: putch('B');break;
	case 12: putch('C');break;
	case 13: putch('D');break;
	case 14: putch('E');break;
	case 15: putch('F');break;
	default: putch('X');
	}
*/
//s+=0x200;
//s=0x200;
//if (s)
//__asm__ volatile("movw %0,%%ax"::"r"(s):"%ax");
    while (*s)
	putch(*s++);
}

void putch(char c)
{
__asm__ volatile("movb $0x0E,%ah");
__asm__ volatile("movb %0,%%al"::"r"(c):"%al");
__asm__ volatile("xorb %bh,%bh");
__asm__ volatile("int $0x10");

}

void stop(void)
{
__asm__ volatile("int $0x20");
}
