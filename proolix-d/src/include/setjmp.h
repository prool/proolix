/*      setjmp.h

        Defines typedef and functions for setjmp/longjmp.

        Copyright (c) Serge Pustovoitoff 1996-1997

*/

#ifndef _SETJMP
#define _SETJMP
typedef struct {
        unsigned        j_sp;
        unsigned        j_ss;
        unsigned        j_flag;
        unsigned        j_cs;
        unsigned        j_ip;
        unsigned        j_bp;
        unsigned        j_di;
        unsigned        j_es;
        unsigned        j_si;
        unsigned        j_ds;
        unsigned        j_ax; /* proolix */
        unsigned        j_bx; /* proolix */
        unsigned        j_cx; /* proolix */
        unsigned        j_dx; /* proolix */
}       jmp_buf[1];

void    far  longjmp (jmp_buf jmpb, int retval);
int     far  setjmp  (jmp_buf jmpb);
#endif
