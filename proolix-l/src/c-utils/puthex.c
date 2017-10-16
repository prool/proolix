void puthex1(char c)
{
if (c>9) putch('A'+c-10);
else putch('0'+c);
}

void puthex_b(char c)
{
puthex1((c>>4)&0xFU);
puthex1(c&0xFU);
}

void puthex(int c)
{
puthex_b((c>>8) & 0xFFU);
puthex_b(c&0xFFU);
}

void puthex_l(int c)
{
puthex ((c>>16)&0xFFFFU);
puthex (c & 0xFFFFU);
}
