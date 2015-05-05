#include <prool.h>

#include <conf.c>
#include "kernel.h"

void interrupt int1b (void)
/* Interrupt 1B - ctrl-break pressed */
{int i;
/*        pushf                 */ /* old fl = [BP+22] */
/*        push cs               */ /* old cs = [BP+20] */
/*        push ip               */ /* old ip = [BP+18] */

/*        push    ax            */ /* old ax = [BP+16] */
/*        push    bx            */ /* old bx = [BP+14] */
/*        push    cx            */ /* old cx = [BP+12] */
/*        push    dx            */ /* old dx = [BP+10] */
/*        push    es            */ /* old es = [BP+ 8] */
/*        push    ds            */ /* old ds = [BP+ 6] */
/*        push    si            */ /* old si = [BP+ 4] */
/*        push    di            */ /* old di = [BP+ 2] */
/*        push    bp            */ /* old BP = [BP]    */
/*        mov     bp,DGROUP     */
/*        mov     ds,bp         */
/*        mov     bp,sp         */
/*        sub     sp,2          */

asm     mov     ax,[BP+20];
asm     mov     i,ax;
printf("CS:IP = %04X",i);
asm     mov     ax,[BP+18];
asm     mov     i,ax;
printf(":%04X",i);

asm     mov     ax,[BP+22];
asm     mov     i,ax;
printf(" FL=%04X",i);

asm     mov     ax,[BP+6];
asm     mov     i,ax;
printf(" DS=%04X",i);

asm     mov     ax,SS;
asm     mov     i,ax;
printf(" SS:SP=%04X:",i);

asm     mov     ax,SP;
asm     mov     i,ax;
printf("%04X",i);

}
