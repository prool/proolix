/*      bios.h

        Access to bios services.

        Copyright (c) Serge Pustovoitoff 1993-1997

*/

/* For Proolix only */

/* Interface to int 14h - BIOS comport I/O control */
#ifndef EXE
unsigned char inicom_(unsigned int portid, unsigned char Par);
#endif
unsigned int getcom_(unsigned int Com1);
unsigned int getcomstat_(unsigned int Com1);
unsigned int putcom_(unsigned int Com1, unsigned char Symbol);

int inicom(unsigned int portid, unsigned char Par);
int getcom(int portid);

/* Defines for use int 14h - com-port I/O */

#define Baud110  0x00
#define Baud150  0x20
#define Baud300  0x40
#define Baud600  0x60
#define Baud1200 0x80
#define Baud2400 0xa0
#define Baud4800 0xc0
#define Baud9600 0xe0

#define Bit7 2
#define Bit8 3

#define StopBit1 0
#define StopBit2 4

#define None 0
#define Even 0x18 /* чет   */
#define Odd  8    /* нечет */

/*

Из книги Фролов А.В.,Фролов Г.В. Программирование модемов.-М.:"ДИАЛОГ-МИФИ",
1993.-240с.-(Библиотека системного программиста; Т.4)

Portions Copyright by А.В.Фролов, Г.В.Фролов, 1993.

COM1, COM3 - IRQ4 (INT 0CH)
COM2, COM4 - IRQ3 (INT 0BH)

                ICR     IIDR    LCR     MCR     LSR     MSR
Offset          1       2       3       4       5       6
COM1    3F8     3F9     3FA     3FB     3FC     3FD     3FF
COM2    2F8     2F9     2FA     2FB     2FC     2FD     2FF
COM3    3E8     3E9     3EA     3EB     3EC     3ED     3EF
COM4    2E8     2E9     2EA     2EB     2EC     2ED     2EF

ICR - interrupt control reg
IIDR - interrupt identification reg
LCR - control reg
MCR - modem control reg
LSR - line status reg
MSR - modem status reg

*/

#define ICR     1
#define IIDR    2
#define LCR     3
#define MCR     4
#define LSR     5
#define MSR     6

#define UART_8250 1
#define UART_16450 2
#define UART_16550 3
#define UART_16550A 4

/* Defines for use RS-232 adaptor via iAPX86's I/O ports (IN & OUT
processor's commands */

#define B110    1040
#define B150    768
#define B300    384
#define B600    192
#define B1200   96
#define B2400   48
#define B4800   24
#define B9600   12
#define B19200  6
#define B38400  3
#define B57600  2
#define B115200 1       /* 14 Kbyte per second */

/* Из книги OFF */

void new_video_page (char page);
