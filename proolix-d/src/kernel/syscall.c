/* Реализация механизма системных вызовов */

int ParLen, Alpha, Beta;
int user_ax, user_ss, user_sp;

void interrupt SysCall2()
{
/*
В этой функции imho нельзя использовать локальные переменные. (???)
*/

/* CS сейчас указывает на ядро, а DS/SS/SP на юзерский процесс, вызвавший
данный вызов */
/* Сохранение состояния (регистров) процесса, вызвавшего system call */
/*
;       push    ax
;       push    bx
;       push    cx
;       push    dx
;       push    es
;       push    ds
;       push    si
;       push    di
;       push    bp
;       mov     bp,DGROUP
;       mov     ds,bp
                                ; а здесь и DS указывает на ядро
*/
Mode=0; /* Mode kernel */
asm     mov     bp,sp

asm     mov     user_ax,ax

asm     mov     ax,SS
asm     mov     user_ss,ax

asm     mov     ax,BP; /* В BP лежит нужное для сохранения юзерское SP */
asm     mov     user_sp,ax

#if 0
/* устанавливаем ядерные SS/SP */
asm     cli
asm     mov     ax,50H
asm     mov     SS,ax
asm     mov     ax,0FFFEh
asm     mov     SP,ax
asm     mov     BP,SP
asm     sti
#endif

/* Копирование параметров из стека в стек */
ParLen=SysTable[user_ax].ParNo*2;
asm     sub     SP,ParLen;
asm     mov     Alpha,SS;
asm     mov     Beta,SP;
memcpy(MK_FP(Alpha,Beta), MK_FP(user_ss,user_sp+26), ParLen);
/* Вызов функции, реализующей данный системный вызов */
(SysTable[user_ax].Entry)();
asm     add     SP,ParLen;
asm     mov     Alpha,ax;
asm     mov     Beta,dx;
Mode=1; /* Mode user */

/* Подкладываем в стек, где сохранены регистры, значение, возвращенное
системным вызовом в регистрах AX и DX */
*(int far*)MK_FP(user_ss,user_sp+16)=Alpha;
*(int far*)MK_FP(user_ss,user_sp+10)=Beta;

/*  восстановление юзерских ss sp */
asm     cli
Global=user_ss;
asm     mov     ax,Global
asm     mov     SS,ax
Global=user_sp;
asm     mov     ax,Global;
asm     mov     SP,ax
asm     sti
}
