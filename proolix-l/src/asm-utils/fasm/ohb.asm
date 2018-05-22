; ohb

ohb:

; Procedure output hex byte Ver 0.1.1 6 Dec 93 via BIOS
; Input: AL - byte
; All regs. reserved ;)

; Not worked in graph mode. bl - bg color !!!

        push    ax
        push    cx
        push    dx

        mov     dl,al
        mov     cl,4
        shr     al,cl
        call    ohb1

        mov     al,dl
        and     al,0fh
        call    ohb1

        pop     dx
        pop     cx
        pop     ax

        ret

ohb1:    ; Regs not saved !!!
        push    ax

        cmp     al,9
        ja      @@_1    ; al > 9
        ; al <= 9
        add     al,'0'
        jmp     @@_out

@@_1:   add     al,'A'-10
@@_out: mov     ah,0eh
        int     10h

        pop     ax

        ret
