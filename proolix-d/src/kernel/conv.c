#include <prool.h>

#include <conf.c>
#include "kernel.h"
/****************************************************************************/
/* #define DEBUG */
/****************************************************************************/
char HexDigit [] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int Divisor8  [] = {0100000, 010000, 01000, 0100, 010, 1};
int Divisor10 [] = {10000, 1000, 100, 10, 1};
long Divisor10L [] = {1000000000L,
                      100000000L,
                       10000000L,
                        1000000L,
                         100000L,
                          10000L,
                           1000L,
                            100L,
                             10L,
                              1L};

long Divisor8L [] = {010000000000L,
                     01000000000L,
                      0100000000L,
                       010000000L,
                        01000000L,
                         0100000L,
                          010000L,
                           01000L,
                            0100L,
                             010L,
                              01L};

int htoi(const char far *s)
{
int i; char c;
i=0;
while (*s)
  {
  i<<=4;
  c=toupper(*s++);
  if ((c>='0')&&(c<='9')) i+=c-'0';
  else if ((c>='A')&&(c<='F')) i+=c+10-'A';
  else return 0;
  }
return i;
}
/****************************************************************************/
long htol(const char far *s)
{
long i; char c;
i=0;
while (*s)
  {
  i<<=4;
  c=toupper(*s++);
  if ((c>='0')&&(c<='9')) i+=c-'0';
  else if ((c>='A')&&(c<='F')) i+=c+10-'A';
  else return 0;
  }
return i;
}
/****************************************************************************/
int atoi(const char far *s)
{
int i; char c;
i=0;
if (*s=='+') s++;
else if(*s=='-')return-atoi(++s);
while (*s)
  {
  c=*s++;
  if (isdigit(c)) {i=i*10; i+=c-'0';}
  else break;
  }
return i;
}
/****************************************************************************/
long atol(const char far *s)
{
long i; char c;
i=0;
if (*s=='+') s++;
else if(*s=='-')return-atol(++s);
while (*s)
  {
  c=*s++;
  if (isdigit(c)) {i=i*10; i+=c-'0';}
  else break;
  }
return i;
}
/****************************************************************************/
#if 0
void fuck1(void)
{
int Divisor [] = {10000, 1000, 100, 10, 1};

printf("fuck1 = %04X\n",Divisor[0]);

}
#endif
/****************************************************************************/
char far *itoa (int w, char far *str, int radix)
{
char far *ret;
int i, r, pow, n;

/* printf("itoa = %04X\n",Divisor10[0]); */

if (radix>34) radix=34;

ret=str;
if (w<0) if (radix==10) {*str++='-'; itoa(-w,str,-10); return ret;}

switch(radix)
  {
  case   2: for (i=0;i<16;i++) {*str++='0'+ w & 0x8000; w<<=1;} break;
  case   8: for (i=0;i<6;i++)
              {r=w/Divisor8[i]; w%=Divisor8[i]; *str++='0'+r;}
            break;
  case  16: for (i=0;i<4;i++) {*str++=HexDigit[(w&0xF000)>>12];w<<=4;} break;
  case  10: *str++='0';
            for (i=0;i<5;i++)
              {r=w/Divisor10[i]; w%=Divisor10[i]; *str++='0'+r;
              #ifdef DEBUG
              printf("itoa: Divisor10[i]=%04X r=%04X w=%04X\n",
                Divisor10[i],r,w);
              #endif
              }
            break;
  case -10: for (i=0;i<5;i++)
              {r=w/Divisor10[i]; w%=Divisor10[i]; *str++='0'+r;
              }
            break;
  default:  pow=UINT_MAX;
            for (i=1;i<16;i++) {pow/=radix; if (pow>0) break;}
            n=i; pow=1;
            for (i=1;i<n;i++,pow*=radix);
            for (i=0;i<n;i++)
              {pow/=radix; r=w/pow; w%=pow; *str++=HexDigit[r];}
            break;
  }
*str=0;
return ret;
}
/****************************************************************************/
char far *ltoa (long w, char far *str, int radix)
{
char far *ret;
int i, n;
long r, pow;

if (radix>34) radix=34;

ret=str;
if (w<0) if (radix==10) {*str++='-'; ltoa(-w,str,-10); return ret;}

switch(radix)
  {
  case  2: for (i=0;i<32;i++) {*str++='0'+ w & 0x8000; w<<=1;} break;
  case  8: for (i=0;i<11;i++)
             {r=w/Divisor8L[i]; w%=Divisor8L[i]; *str++='0'+r;}
           break;
  case 16: for (i=0;i<8;i++)
             {*str++=HexDigit[(int)((w&0xF0000000L)>>28)];w<<=4;}
           break;
  case 10: *str++='0';
           for (i=0;i<10;i++)
             {r=w/Divisor10L[i]; w%=Divisor10L[i]; *str++='0'+r;
             }
           break;
  case -10: for (i=0;i<10;i++)
              {r=w/Divisor10L[i]; w%=Divisor10L[i]; *str++='0'+r;
              }
            break;
  default: pow=ULONG_MAX;
           for (i=1;i<32;i++) {pow/=radix; if (pow>0) break;}
           n=i; pow=1;
           for (i=1;i<n;i++,pow*=radix);
           for (i=0;i<n;i++)
             {pow/=(long)radix; r=w/pow; w%=pow; *str++=HexDigit[(int)r];}
           break;
  }
*str=0;
return ret;
}
/****************************************************************************/
char far *ultoa (unsigned long w, char far *str, int radix)
{
char far *ret;
int i, n;
long r, pow;

if (radix>34) radix=34;

ret=str;

switch(radix)
  {
  case  2: for (i=0;i<32;i++) {*str++='0'+ w & 0x8000; w<<=1;} break;
  case  8: for (i=0;i<11;i++)
             {r=w/Divisor8L[i]; w%=Divisor8L[i]; *str++='0'+r;}
           break;
  case 16: for (i=0;i<8;i++)
             {*str++=HexDigit[(int)((w&0xF0000000L)>>28)];w<<=4;}
           break;
  case 10: *str++='0';
           for (i=0;i<10;i++)
             {r=w/Divisor10L[i]; w%=Divisor10L[i]; *str++='0'+r;
             }
           break;
  case -10: for (i=0;i<10;i++)
              {r=w/Divisor10L[i]; w%=Divisor10L[i]; *str++='0'+r;
              }
            break;
  default: pow=ULONG_MAX;
           for (i=1;i<32;i++) {pow/=radix; if (pow>0) break;}
           n=i; pow=1;
           for (i=1;i<n;i++,pow*=radix);
           for (i=0;i<n;i++)
             {pow/=(long)radix; r=w/pow; w%=pow; *str++=HexDigit[(int)r];}
           break;
  }
*str=0;
return ret;
}
