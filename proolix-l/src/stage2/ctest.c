/* fuction prototypes */
void puts0(char *s);
void putch(char);
void putch_tty(char);
void pause(void);
int getch(void);
void puthex1(char c);
void puthex_b(char c);
void puthex(int c);
void puthex_l(int c);
void putdec (int w);
void putdec2 (int w, int digits, int zeroes);

/* main function */
void main (void)
{
//putch_tty('0');
//putch_tty('1');
puts0("\r\nprooltest\r\n");

pause ();

__asm__ volatile("int $0x20"); // STOP
}
/* functions */
void puts0(char *s)
{
    while (*s)
	putch_tty(*s++);
}

void pause(void)
{int c;
	puts0(" Pause. Press any key ");
	c=getch();
	puts0("Keycode "); puthex(c);
	switch(c)
	{
		case ' ': puts0(" SPACE "); break;
		case 'z': puts0(" z "); break;
		default: puts0(" fucking thing ");
	}
}

int getch(void)
{
/*
AH = 00h

Return:
AH = BIOS scan code
AL = ASCII character
*/
__asm__ volatile("xor %ah,%ah");
__asm__ volatile("int $0x16");
__asm__ volatile("xor %ah,%ah");
}

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

void putdec (int w)
{int r,i,trail_zeroes;
trail_zeroes=1;
int Divisor10 [10] = {1000000000,100000000,10000000,1000000,100000,10000, 1000, 100, 10, 1};

if (w<0) {putch('-'); w=-w;}
if (w==0) {putch('0'); return;}

            for (i=0;i<10;i++)
              {
              r=w/Divisor10[i];
              w%=Divisor10[i];
              if (r!=0)
            	    {
            	    putch('0'+r);
            	    trail_zeroes=0;
            	    }
              else // r==0
            	    if (trail_zeroes)
            		{// nothing
            		}
            	    else
            		{
            		putch('0');
            		}
              }
}

void putdec2 (int w, int digits, int zeroes)
{int r,i,trail_zeroes,j;
trail_zeroes=1;
int Divisor10 [10] = {1000000000,100000000,10000000,1000000,100000,10000, 1000, 100, 10, 1};

if ((digits<1)||(digits>10)) return;

if (w<0) {putch('-'); w=-w;}
if (w==0) {for(i=0;i<digits;i++) putch('0'); return;}

            for (i=10-digits;i<10;i++)
              {
              r=w/Divisor10[i];
              w%=Divisor10[i];
              if (r>9)
        	    {// overflow
        	    for (j=0;j<digits;j++) putch('*');
        	    return;
        	    }
              if (r!=0)
            	    {
            	    putch('0'+r);
            	    trail_zeroes=0;
            	    }
              else // r==0
            	    if (trail_zeroes)
            		{if (zeroes==0) putch('0');
			else putch(' ');
            		}
            	    else
            		{
            		putch('0');
            		}
              }
}

void putch(char c)
{
	putch_tty(c);
}
