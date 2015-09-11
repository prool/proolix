#include <prool.h>

#include <dos.h>

#include <conf.c>
#include "kernel.h"
/****************************************************************************/
int int86 (int intno, union REGS far *inregs, union REGS far *outregs)
{int i;
i=(*inregs).x.ax;
/*
        mov     bx,word ptr [bp+6]
        mov     ax,word ptr [bx]
        mov     word ptr [bp-2],ax
        */
i=(*inregs).x.bx;
/*
        mov     bx,word ptr [bp+6]
        mov     ax,word ptr [bx+2]
        mov     word ptr [bp-2],ax
        */
i=(*inregs).h.al;
/*
        mov     bx,word ptr [bp+6]
        mov     al,byte ptr [bx]
        mov     ah,0
        mov     word ptr [bp-2],ax
        */

}
/****************************************************************************/
/*
int int86x (int intno, union REGS *inregs, union REGS *outregs,
                         struct SREGS *segregs)
{
}
  */
