#include <conf.c>

#ifndef EXE
#include "kernel.h"
#include <dos.h>
#else
#include <dos.h>
#endif

#include <bios.h>

/*---------------------------------------------------------------------------*/
/*

Из книги Фролов А.В.,Фролов Г.В. Программирование модемов.-М.:"ДИАЛОГ-МИФИ",
1993.-240с.-(Библиотека системного программиста; Т.4)

*/
unsigned com_address ( int portid ) {
  unsigned base_address;

  if(( portid > 4) || ( portid < 0 )) return( -1 );
  base_address =
    *(( unsigned far * ) MK_FP( 0x40, portid * 2 ));
  return( base_address );
}
/* Из книги OFF */
/*---------------------------------------------------------------------------*/
#ifndef EXE
int inicom (unsigned int portid, unsigned char Par)
{int i;
int     ComBase;

i=inicom_(portid,Par);
if((ComBase=com_address(portid))==0)return -1;
outp (ComBase+LCR, inp((ComBase+LCR) | 0x80) );
outp (ComBase+ICR,0);
return i;
}
#endif
/*---------------------------------------------------------------------------*/
int getcom(int portid)
{int ComBase;

if((ComBase=com_address(portid))==0) return -1;
if ( inp(ComBase+LSR) & 1 ) return inp(ComBase);
else return -2;
}
/*---------------------------------------------------------------------------*/
int putcom(int portid, unsigned char byte)
{int ComBase;

if((ComBase=com_address(portid))==0) return -1;
while (( inp(ComBase+LSR) & 0x20 )==0);
outp(ComBase,byte);
return 0;
}
/*---------------------------------------------------------------------------*/
int getcomstat(int portid)
{int ComBase;

if((ComBase=com_address(portid))==0) return -1;
return inp(ComBase+LSR);
}
/*---------------------------------------------------------------------------*/
int is_UART_8250 (int port ) {
int save_scr, in_scr;
int addr;
int ComBase;

ComBase=com_address(port);

#ifdef EXE
if (ComBase) printf("%4X",ComBase);
#endif

if(ComBase==0)return -1;

addr=ComBase+(0x3ff-0x3f8);

save_scr=inp(addr);
outp(addr,0x5a);
in_scr=inp(addr);
if(in_scr!=0x5a){
outp(addr,save_scr);
return UART_8250;
}
outp(addr,0xa5);
in_scr=inp(addr);
outp(addr,save_scr);
if(in_scr!=0xa5)
return UART_8250;
return 0;
}
/*---------------------------------------------------------------------------*/
int is_UART_FIFO(int port) {
/*int save_iir;*/
int in_iir;
int addr;
int ComBase;

if((ComBase=com_address(port))==0) return -1;

addr=ComBase+(0x3fa-0x3f8);

/*save_iir=inp(addr);*/
outp(addr,1);
in_iir=inp(addr);
outp(addr,0);
if((in_iir & 0x40) == 1)
return UART_16550A;
if((in_iir&0x80)==1)
return UART_16550;
return UART_16450;
}
/*---------------------------------------------------------------------------*/
int uart_type (int port)
{int test;

if( (test=is_UART_8250(port)) == UART_8250 ) return UART_8250;
else if (test==-1) return -1;

if(( test = is_UART_FIFO(port) ) == UART_16550A ) return UART_16550A;

if(test == UART_16550) return UART_16550;

return UART_16450;

}

#ifdef EXE
main()
{int i;
char * UARTname [] = {
"8250   without FIFO",
"16450  without FIFO",
"16550  with FIFO",
"16550A with FIFO"};

clrscr();
puts("Port Base addr UART type");
printf(  "COM1 ");if((i=uart_type(0))!=-1)printf(UARTname[i-1]);else printf("Not found");
printf("\nCOM2 ");if((i=uart_type(1))!=-1)printf(UARTname[i-1]);else printf("Not found");
printf("\nCOM3 ");if((i=uart_type(2))!=-1)printf(UARTname[i-1]);else printf("Not found");
printf("\nCOM4 ");if((i=uart_type(3))!=-1)printf(UARTname[i-1]);else printf("Not found");
puts("");
}
#endif
