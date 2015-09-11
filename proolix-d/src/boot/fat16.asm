; for FAT-16
NextClu proc    ; Input: ax - cluster no
                ; Output: ax - next cluster from FAT
                ; cf=1 if EOF
                ; REGS SAVED !

        push    bx
        push    dx
        push    ES

        mov     bx,2
        mul     bx       ; dx:ax := ax * bx
        ; dx:ax - relative address in FAT
        add     dx,FATseg
        ; dx:ax - absolute address in FAT
        mov     ES,dx
        mov     bx,ax

        mov     ax,word ptr ES:[bx]
;       cmp     ax,0fffh
;       je      @@cf
;       cmp     ax,0ff7h
;       je      @@cf

        cmp     ax,word ptr MaxClusters+off
        ja      @@cf

        clc
@@ret:
        pop     ES
        pop     dx
        pop     bx

        ret
@@cf:   stc
        jmp     @@ret
NextClu endp
