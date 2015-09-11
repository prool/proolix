; MSHerc.com by Microsoft Corp. (?)
; Disassembled by Sourcer V 5.04 (C) V COMMUNICATIONS, INC. 1988-1993
; Copyright (C) Comments & adaptation for Proolix by Serge Pustovoitoff, 1995

; à≠®Ê®†´®ß†Ê®Ô Ì¨„´ÔÊ®Ô CGA ≠† É•‡™„´•·• (·¨. msherc.asm)
; èÆ´„Á•≠Æ §®ß†··•¨°´®‡Æ¢†≠®•¨ ® ¨Æ§®‰®™†Ê®•© Ø‡Æ£‡†¨¨Î msherc.com

;PAGE  ,78

;€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€
;€€                                                                      €€
;€€                             MSHERC                                   €€
;€€                                                                      €€
;€€      Created:   7-Mar-88                                             €€
;€€      Passes:    9          Analysis Options on: none                 €€
;€€                                                                      €€
;€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€€

; The following equates show data references outside the range of the program.

Vec10h          equ     10h*4

_TEXT   segment byte public 'CODE'
        assume  cs:_TEXT

;ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ
;                              MAIN
;‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹

_mshercinit     proc
                public  _mshercinit
; int mshercinit (void);

                ; save regs
                push    bx
                push    cx
                push    ES

                extrn   TestHerc:near
                call    TestHerc   ; Ø‡Æ¢•‡™† ≠† ≠†´®Á®• É•‡™„´•·†
                cmp     dl,0FFh
                jne     HercPresent
                ; Herc not Present
                mov     ax,-1
                jmp     @@ret

HercPresent:
                ; save old vector of int 10h
                extrn   OldVec10h:word

                xor     ax,ax
                mov     es,ax                   ; ES:=0
                mov     bx,es:Vec10h            ; bx := 0:40
                mov     word ptr OldVec10h,bx
                mov     bx,es:Vec10h+2
                mov     word ptr OldVec10h+2,bx
                ; set new vector of int 10h
                cli
                extrn   msherc:near
                mov     word ptr es:Vec10h,offset msherc
                mov     es:Vec10h+2,cs
                sti
                xor     ax,ax
@@ret:
                ; restore vec
                pop     ES
                pop     cx
                pop     bx

                ret

           endp
           ends
           end
