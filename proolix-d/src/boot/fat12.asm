; for FAT-12
NextClu proc    ; Input: ax - cluster no
                ; Output: ax - next cluster from FAT
                ; cf=1 if EOF
                ; REGS SAVED !

        push    bx
        push    cx
        push    dx
        push    di
        push    ES

        mov     di,ax
        mov     bx,3
        mul     bx      ; dx:ax := ax * bx
        shr     ax,1    ; ax := ax /2
        ; ax - addr in FAT
        mov     bx,FATseg
        mov     ES,bx
        mov     bx,ax
        mov     ax,word ptr ES:[bx]
        test    di,1
        jz      @@1
        mov     cl,4
        shr     ax,cl
        jmp     @@2
@@1:    and     ax,0fffh
@@2:
;       cmp     ax,0fffh
;       je      @@cf
;       cmp     ax,0ff7h
;       je      @@cf

        cmp     ax,word ptr MaxClusters+off
        ja      @@cf

        clc
@@ret:
        pop     ES
        pop     di
        pop     dx
        pop     cx
        pop     bx

        ret
@@cf:   stc
        jmp     @@ret
NextClu endp
