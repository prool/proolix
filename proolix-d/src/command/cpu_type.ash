; ----------------------------------------------------------------------------
; CPU_TYPE.ASH  Too-Much-In-One-So-Don't-Get-Lost(tm)
;               CPU/FPU feature detection library               Version 1.14
;
; Copyright(c) 1992,93,94 by B-coolWare.    Written by Bobby Z.
; Portions copyright(c) 1990 by 2B Programmers
; All rights reserved.
; ----------------------------------------------------------------------------
; Uses:
;       UNIDEF.INC      universal macros and defines
;       LSTRING.ASH     Lstring macro
;       DOSINOUT.ASH    WriteStr routine
;
; History:
;
;       Dec 17 1992     initially written
;       Jun 10 1993     added distinguish between 80386 and i486
;       Sep 14 1993     added Pentium and above CPUs detection
;                       added 386sx and 386dx distinguishing
;                       added i486sx, i486dx and Cyrix 486 distinguishing
;                       added FPU distinguishing routine
;       Sep 16 1993     added i486slc distinguishing
;       Nov 23 1993     fixed Pentium fixup bug
;                       fixed i386step bug (i hope so)
;       Jan 10 1994     added IIT xC87 math coprocessors detection
;       Jan 12 1994     added some comments and fixed spelling errors - no
;                       changes to code made
;       Jan 18 1994     fixed bug that causes fpu_type routine to hang on 486s
;       Mar 23 1994     distinguish between 486dx and 486sx made reliable
;                       added 386SL detection routine (ain't been tested yet)
;       Jun 07 1994     Test_Buffer made reentrant - I just forgot to do
;                       that before...
;       Jun 20 1994     added Cyrix M1 (586) identification, FPU_Type routine
;                       slightly corrected.


EF_AC           equ     00040000h       ; AC bit in EFLAGS register
EF_ID           equ     00200000h       ; ID bit in EFLAGS register
MSW_ET          equ     00000010h       ; ET bit in MSW register
MSW_NE          equ     00000020h       ; NE bit in MSW register

; following constants are returned in AX by CPU_Type routine

cpu8088         equ     0000h   ; 8088
cpu8086         equ     0001h   ; 8086
cpuNECV20       equ     0002h   ; NEC V20
cpuNECV30       equ     0003h   ; NEC V30
cpu80188        equ     0004h   ; 188
cpu80186        equ     0005h   ; 186
cpu80286        equ     0006h   ; 286
cpu286          equ     06h     ; 286
cpu80386sxv     equ     0107h   ; 386sx in V86 mode
cpu80386sxr     equ     0007h   ; 386sx in real mode
cpu386dx        equ     08h     ; 386dx
cpu80386dxv     equ     0108h   ; 386dx in V86 mode
cpu80386dxr     equ     0008h   ; 386dx in real mode
cpu80386slv     equ     0109h   ; 386sl in V86 mode
cpu80386slr     equ     0009h   ; 386sl in real mode
cpui486sxv      equ     010Ah   ; 486sx in V86 mode
cpui486sxr      equ     000Ah   ; 486sx in real mode
cpu486dx        equ     0Bh     ; 486dx
cpui486dxv      equ     010Bh   ; 486dx in V86 mode
cpui486dxr      equ     000Bh   ; 486dx in real mode
cpuCx486        equ     0Ch     ; Cyrix 486
cpu486slcv      equ     010Ch   ; Cyrix 486slc in V86
cpu486slcr      equ     000Ch   ; Cyrix 486slc in real mode
cpu486dlc       equ     0Dh
cpu486dlcv      equ     010Dh   ; Cyrix 486dlc in V86
cpu486dlcr      equ     000Dh   ; Cyrix 486dlc in real mode
cpuPentium      equ     0Eh
cpuPentiumr     equ     000Eh   ; Pentium in real mode
cpuPentiumv     equ     010Eh   ; Pentium in V86 mode
cpuCxM1         equ     0Fh
cpuCxM1r        equ     000Fh   ; Cyrix M1 (586) in real mode
cpuCxM1v        equ     010Fh   ; Cyrix M1 (586) in V86 mode
cpuP6           equ     0010h   ; add new CPUs here

        LOCALS
        JUMPS

cpuid   equ     <db     0Fh,0A2h>       ; Pentium instruction

public  CPU_Type

CPU_Type        proc

LOCAL   ETFlipped : BYTE

; entry - none
; returns AL = CPU code (see above constants)
;         AH = 1 if in V86, 0 otherwise
;         DL = FPU code (see below constants)

        push    bx cx si
        clr     bx
        push    sp              ; this code uses bug in chips prior to
        pop     ax              ; 80286: when push sp performed, value
        cmp     ax,sp           ; of sp is first decremented and then
                                ; placed onto the stack. 286 and up
                                ; handle this instruction correctly,
                                ; saving value which sp have upon issue
                                ; of this command, not after.
        jnz     @@000           ; if not equal that it is <286
        mov     ax,7000h
        pushf
        push    ax
        popf
        pushf
        pop     ax
        popf
        mov     bl,6
        and     ax,7000h        ; check for flag - only 386+ has it
        jz      @@200           ; if ax=0 than this is 286
        inc     bx

; distinguish between 386SX/Cx486SLC and 386DX/Cx486DLC chips

; This one uses the fact, that 386sx/Cx486SLC has 16-bit data bus and thus
; uses only 16-bit bus interchange format to communicate with coprocessor.
; Due to this limitation the ET bit in MSW is always zero and can't be
; changed, but on 386dx/Cx486DLC it can be flipped.


        .386p
        clr     si
        mov     eax,cr0
        mov     ecx,eax
        xor     eax,MSW_ET      ; flipping ET bit
        mov     cr0,eax
        mov     eax,cr0
        mov     cr0,ecx         ; restoring previous value of CR0
        xor     eax,ecx         ; did it flip ok?
        jz      @@L100
        inc     si              ; DX/DLC
@@L100:

;This code that distinguishes a 386 from a 486 depends on
;the 386's inability to toggle the AC bit in the EFLAGS register,
;but the 486 can. This technique is apparently blessed by Intel.

        ;Distinguish between 386 and 486
        ;Placed into public domain by Compaq Computers.

        .386

        mov     ax,sp
        and     sp,0FFFCh       ;round down to a dword boundary
        pushfd
        pushfd
        pop     edx
        mov     ecx,edx
        xor     edx,EF_AC       ;toggle AC bit
        and     ecx,EF_AC
        push    edx
        popfd
        pushfd
        pop     edx
        popfd                   ;restore original flags
        mov     sp,ax           ;restore original stack pointer
        and     edx,EF_AC

        cmp     edx,ecx
        jnz     @@486           ;it's a 486
        or      si,si           ; SX/SLC?
;       jz      @@386sl
        jz      @@L1
        inc     bx              ; 386DX
        jmp     @@L1
;@@386sl:                       ; tested as SX - checking if SL
;       call    check386sl
;       jnc     @@L1
;       inc     bx
;       inc     bx
;       jmp     @@L1

@@486:
        ; distinguish between Cyrix 486 and Intel 486+
        mov     bx,cpui486sxr
        push    bx
        mov     cx,8D5h
        clr     ax,ax
        mov     dx,ax
        cmp     ax,ax
        pushf
        mov     ax,0FFFFh
        mov     bx,4
        div     bx
        pushf
        pop     ax dx bx
        and     ax,cx
        and     dx,cx
        cmp     ax,dx
        jnz     @@586
        inc     bx              ; Cyrix 486SLC
        inc     bx
        or      si,si
        jz      @@586
        inc     bx              ; Cyrix 486DLC
@@586:

; Check for Pentium or later by attempting to toggle the Id bit in EFLAGS reg:
; if we can't, it's an i486.

        ; Pentium detection routine
        ; Placed in public domain by Dr. Dobbs Journal

        pushfd                  ; get current flags
        pop     eax
        mov     ecx,eax
        xor     eax,EF_ID       ; attempt to toggle ID bit
        push    eax
        popfd
        pushfd                  ; get new EFLAGS
        pop     eax
        push    ecx             ; restore original flags
        popfd
        and     eax,EF_ID       ; if we couldn't toggle ID,
        and     ecx,EF_ID       ; then this is i486
        cmp     eax,ecx
        jz      @@486sdx        ; do not alter BX
; It's Pentium or later. Use CPUID to get processor family.
        sub     eax,eax         ; get processor info
        inc     al
        cpuid
        and     ah,0Fh          ; 5 means Pentium
        cmp     ah,4
        jb      @@386dx         ; CPUID also works on later models of 386dx
        je      @@486dx         ; and 486dx. Though we shouldn't come here on
                                ; a 386 it's better to make an extra check for
                                ; the case we occasionally did.
        cmp     bl,cpuCx486     ; was Cyrix microcode detected ?
        jb      @@P5
        cmp     bl,cpuCx486+1
        ja      @@P5
        mov     bl,cpuCxM1      ; yes - this is Cyrix M1 chip
        jmp     @@L1
@@P5:
        mov     bl,ah
        add     bl,cpuPentium-5
        jmp     @@L1
@@386dx:
        mov     bx,cpu80386dxr
        jmp     @@L1
@@486sdx:
        ; distinguish between i486dx and i486sx processors
        ; based on 486sx's inability to toggle NE bit of MSW

        .486p
        mov     eax,cr0
        mov     ecx,eax
        db      66h,83h,0E0h,0DFh
;       and     eax,0FFFFFFDFh  ; flip off NE bit of MSW
        mov     cr0,eax
        mov     eax,cr0
        cmp     eax,ecx
        jnz     @@486dx
        or      eax,MSW_NE      ; flip on NE bit of MSW
        mov     cr0,eax
        mov     eax,cr0
        cmp     eax,ecx
        jnz     @@486dx
        dec     bx
@@486dx:
        inc     bx
        mov     eax,ecx
        mov     cr0,eax

@@L1:
        .286p
        smsw    ax
        and     al,1
        mov     bh,al           ; get the VM flag into bh
        jmp     @@200
@@000:
        mov     bl,4            ; assume this is 186/188
        mov     cl,33
        clr     ax
        dec     ax
        shl     ax,cl
        jnz     @@100           ; 186/188 does not actually shift
                                ; more that 32 bits. It shifts only
                                ; n mod 32 bits, where n is number of
                                ; bits to shift.
        mov     bl,2            ; assume NEC family
        clr     cx
        dec     cx
;       rep  es: lodsb  - incorrect order of prefixes and thus would
;                         not work as repeated command on Intel's CPUs.
;                         8086/88 accepts only es: rep lodsb order. But
;                         NEC Vxx CPUs process multiple prefix repeated
;                         instructions correctly regardless of their
;                         order.

        db      0F3h,26h,0ACh
;               rep  es: lodsb

        jcxz    @@100           ; was repeated cx times -> NEC V20/V30
        sub     bx,bx           ; good old 88/86
@@100:
        call    Test_Buffer
        jcxz    @@200           ; prefetch buffer length < 6 bytes -> 88 / V20
        inc     bx              ; prefetch buffer length = 6 bytes -> 86 / V30
@@200:
        call    FPU_Type
        mov     ax,bx
        pop     si cx bx
        ret
        endp

;check386sl     proc
; CF = 1 if 386SL
; meaning of this code is unclear for me, but Diagsoft states this works
; ok.
;       cli
;       in      ax,22h
;       mov     cx,ax
;       test    cl,1
;       jnz     @@1
;       mov     ax,8000h
;       out     23h,al
;       xchg    ah,al
;       out     22h,al
;       out     22h,ax
;       jmp     $+2
;       in      ax,22h
;       test    al,1
;       jz      @@2
;@@1:
;       mov     ax,cx
;       or      ah,1
;       out     22h,ax
;       jmp     $+2
;       jmp     $+2
;       in      ax,22h
;       test    al,1
;       jnz     @@2
;       mov     ax,8000h
;       out     23h,al
;       xchg    ah,al
;       out     22h,al
;       out     22h,ax
;       jmp     $+2
;       in      ax,22h
;       test    al,1
;       jz      @@2
;       mov     ax,cx
;       test    al,1
;       jnz     @@3
;       or      ah,1
;       out     22h,ax
;       stc
;       jmp     @@Q
;@@2:
;       mov     ax,cx
;       out     22h,ax
;@@3:
;       clc
;@@Q:
;       sti
;       ret
;       endp

Test_Buffer     proc    ; (C) 2B Programmers
        push    es di
        mov     _bpcs[@@0],41h  ; to make this routine reentrant
        std
        push    cs
        pop     es
        ldi     @@2
        mov     al,_bpcs[@@1]
        mov     cx,3
        cli
        rep     stosb
        cld             ; 1
        nop             ; 2
        nop             ; 3
        nop             ; 4     <- 80x88 will cut here and inc cx instruction
@@0:    inc     cx      ; 5        will be overwritten by sti, else we'll get
@@1:                    ;          cx = 1, which indicates 80x86
        sti             ; 6
@@2:
        sti
        pop     di es
        ret
        endp

;       db      13,10
;       db      '                               Too much is not enough...',13,10
;       db      '                                       (Deep Purple)',13,10
;       db      13,10
;       db      'TMIOSDGL(tm) CPU/FPU feature detection library  Version 1.14',13,10
;       db      'Copyright(c) 1992,93,94 by B-coolWare. Released as freeware.',13,10

fsbp0   equ     <db     0DBh,0E8h>      ; IIT xC87 specific instruction:
                                        ; select register bank #0
fsbp1   equ     <db     0DBh,0EBh>      ; IIT xC87 specific instruction:
                                        ; select register bank #1
fsbp2   equ     <db     0DBh,0EAh>      ; IIT xC87 specific instruction:
                                        ; select register bank #2
fmul4x4 equ     <db     0DBh,0F1h>      ; IIT xC87 specific instruction:
                                        ; matrix multiplication


FPU_Type        proc
;
; on entry : BL = CPU code      !! Required !!
;
        .8086
        .8087                   ; check for coprocessor
        mov     dl,2            ; assume no coprocessor present
        finit
        clr     cx
        mov     fpubuf_w,5A5Ah
        fnstsw  fpubuf_w
        fwait
        mov     ax,fpubuf_w
        or      al,al           ; FPU wasn't initialized - no FPU at all
        jnz     @@L15
        fnstcw  fpubuf_w        ; check the control word also
        fwait
        mov     ax,fpubuf_w
        and     ax,103Fh
        cmp     ax,3Fh
        jne     @@L15
        mov     dl,4            ; assume 8087
        fstenv  fpuenv
        fwait
        and     fpubuf_w,0FF7Fh
        fldcw   fpubuf_w
        fwait
        fdisi
        fstcw   fpubuf_w
        fwait
        test    fpubuf_w,80h
        jnz     @@L15
        mov     dl,8            ; assume 80287
        .286
        .287
        finit                   ; checking if -Inf <> +Inf
        fld1                    ; 287 erroneously claim that they are equal
        fldz
        fdivp   st(1),st
        fld     st
        fchs
        fcompp
        fstsw   fpubuf_w
        fwait
        mov     ax,fpubuf_w
        sahf
        jz      @@L15           ; -Inf <> +Inf -> 287XL or 387 and up
        mov     dl,0Ch          ; assume 80387
@@L14:
        .286
        .287
        cmp     bl,09h          ; 486 or up?
        jb      @@checkIIT      ; IIT x87's bank switching instructions causes
        jmp     @@L35           ; 486s to hang... don't know why.

@@checkIIT:
                                ; testing for IIT ?C87
        finit                   ; trying to perform a matrix multiplication
        fsbp1
        wait
        fldz                    ; loading matrix coeffs in bank #1
        fld1
        fldz
        fldz
        fld1
        fldz
        fldz
        fldz
        wait
        finit
        fsbp2
        fldz                    ; loading matrix coeffs in bank #2
        fldz
        fldz
        fld1
        fldz
        fldz
        fld1
        fldz
        wait
        finit
        fsbp0
        wait
        fldz                    ; generating vector
        fld1
        fld     st(0)
        fadd    st,st(0)
        fld     st(0)
        fadd    st,st(2)
        fmul4x4                 ; do the multiplication
        wait
        fstp    iit1            ; store resulting vector...
        fstp    iit2
        fstp    iit3
        fstp    iit4
        wait                    ; ...and check the result
        cmp     _wp [iit4+2],4040h
        jnz     @@L35
        cmp     _wp [iit3+2],4000h
        jnz     @@L35
        cmp     _wp [iit2+2],3F80h
        jnz     @@L35
        cmp     _wp [iit1],0
        jnz     @@L35
                                ; wow! it works - IIT chip
        cmp     dl,0Ch          ; tested as 80387?
        jnz     @@300
        mov     dl,24           ; this is IIT 3C87
        jmp     @@L15
@@300:
        mov     dl,22           ; this is IIT 2C87
        jmp     @@L15
@@L35:
        finit
        fldpi
        f2xm1
        fstp    fpubuf_d
        cmp     word ptr [fpubuf_d+2],3FC9h
        jne     @@L15
        or      dl,2            ; this is Cyrix ?C87
@@L15:
        cmp     bl,7
        jb      @@L16
        .386
        clr     eax
        int     11h
        test    eax,1000000h
        .8086
        jz      @@L16
        or      dl,1            ; Weitek 1167 present
@@L16:
        cmp     bl,cpu486dx     ; 486dx?
        jne     @@L17
@@builtin:
        and     dl,3
        or      dl,10h          ; built-in coprocessor
        jmp     @@nobuilt
@@L17:
        cmp     bl,cpu486dlc    ; 486dlc and Pentium-like CPUs got built-in
        jae     @@builtin       ; FPUs
@@nobuilt:
        cmp     bl,cpu286       ; 286...?
        jnz     @@20
        cmp     dl,0Ch          ; ...and FPU tested as 387...?
        jz      @@21
        cmp     dl,0Dh
        jnz     @@20
@@21:
        add     dl,8            ; then assume 80287XL - tricky
@@20:
        cmp     bl,cpuPentium
        jb      @@30
        and     dl,3
        or      dl,10h          ; built-in coprocessor
@@30:
        cmp     dl,4            ; any 87 present?
        jb      @@L18
        fldenv  fpuenv  ; yes - restore 87 environment
@@L18:
        ret
        endp
fpubuf_w        dw      ?
fpubuf_d        dd      ?
fpuenv          db      14 dup(?)
iit1            dd      6F772049h       ; this is just a message - anything
iit2            dd      7265646Eh       ; can be here.
iit3            dd      20666920h
iit4            dd      00544949h



ifdef   __PRINT_CPU__

CPUs    dw      offset I88
        dw      offset I86
        dw      offset V20
        dw      offset V30
        dw      offset I188
        dw      offset I186
        dw      offset I286
        dw      offset I386SX
        dw      offset I386DX
        dw      offset I386SL
        dw      offset I486SX
        dw      offset I486DX
        dw      offset Cyr486SLC
        dw      offset Cyr486DLC
        dw      offset I586
        dw      offset CxM1

Lstring I88,<Intel 8088>
Lstring I86,<Intel 8086>
Lstring V20,<NEC V20>
Lstring V30,<NEC V30>
Lstring I188,<Intel 80188>
Lstring I186,<Intel 80186>
Lstring I286,<Intel 80286>
Lstring I386SX,<Intel 80386sx>
Lstring I386DX,<Intel 80386dx>
Lstring I386SL,<IBM 386SL>
Lstring I486SX,<Intel i486sx>
Lstring I486DX,<Intel i486dx>
Lstring Cyr486SLC,<Cyrix 486SLC>
Lstring Cyr486DLC,<Cyrix 486DLC>
Lstring I586,<Intel Pentium>
Lstring CxM1,<Cyrix M1 (586)>
Lstring RealMode,< in real mode>
Lstring VirtMode,< in V86 mode>
FPUType dw      ?

print_CPU       proc
        .8086
        call    CPU_Type
        clr     dh
        mov     FPUType,dx
        push    ax
ifdef   __use_386__
        movzx   bx,al
else
        mov     bl,al
        clr     bh
endif
        shl     bx,1
        mov     dx,CPUs[bx]
        call    WriteStr
        pop     ax
        cmp     al,7
        jb      @@Exit
        push    ax
        or      ah,ah
        jnz     @@Virt
        ldx     RealMode
        jmp     @@Pr
@@Virt:
        ldx     VirtMode
@@Pr:
        call    WriteStr
        pop     ax
ifdef   __PRINT_STEP__
        cmp     al,cpu386dx     ; 386dx ?
        jnz     @@Exit
        call    print_Step
endif   ; __PRINT_STEP__
@@Exit:
        ldx     EndLine
        call    WriteStr
ifdef   __PRINT_FPU__
        call    print_FPU
        ldx     EndLine
        call    WriteStr
endif
        ret
        endp

endif   ; __PRINT_CPU__

ifdef   __PRINT_STEP__
__386STEP__     EQU     1
endif

ifdef   __386STEP__

stepB0  EQU     0
stepB1  EQU     1
stepD0  EQU     2

ifdef   __PRINT_STEP__

stepTable       label   word
        dw      offset B0Str
        dw      offset B1Str
        dw      offset D0Str

Lstring B0Str,<B0 or earlier>
Lstring B1Str,<B1>
Lstring D0Str,<D0 or later>
Lstring stepStr,<, 386 chip step is >

print_Step      proc
        .8086
        ldx     stepStr
        call    WriteStr
        call    i386step
ifdef   __use_386__
        movzx   bx,al
else
        mov     bl,al
        clr     bh
endif
        shl     bx,1
        mov     dx,stepTable[bx]
        call    WriteStr
        ret
        endp

endif

; this routine determines 386DX chip stepping.
; ripped from Phar Lap TellMe utility.

; returns:
;               ax = 0  ->  chip is step B0 or earlier
;               ax = 1  ->  chip is step B1
;               ax = 2  ->  chip is step D0 or later

i386step        proc
        .386
        push    bx cx dx si di es ds
        mov     ax,3506h
        int     21h
        push    cs
        pop     ds
        mov     dx,offset @@int06h
        mov     ax,2506h
        int     21h
        clr     cx
        inc     cx
;       .486
;       cmpxchg ds:[bx+si],al   ; this command sequence cause INT 06
        db      0Fh,0A6h,00h    ; to occur on 386s later than step B0.

                                ; TASM does not generate this code when
                                ; using mnemonics. Generated code
                                ; differs from that specified in db
                                ; but decodes in the same instruction.
@@B0:
        mov     dx,bx
        push    es
        pop     ds
        mov     ax,2506h
        int     21h
        jcxz    @@checkB1
        clr     ax
        jmp     @@Qstep
@@int06h:
        clr     cx
        add     sp,4
        popf
        jmp     @@B0
@@buf   dw      0
@@checkB1:
        mov     di,offset @@buf
        mov     dx,80h
        inc     cx              ; cx = 0 if we got here
        mov     ax,cx
        inc     ax
        cld
        push    ax
        rep     insb            ; repeating insb 1 times on B1 step chip does
                                ; not reset cx to zero.
        pop     ax
        jcxz    @@Qstep
        dec     ax
@@Qstep:
        pop     ds es di si dx cx bx
        ret
        endp

endif

ifdef   __PRINT_FPU__

FPUs    dw      offset fpuUnknown       ; ?
        dw      offset fpuUnknown       ; ?
        dw      offset fpuNone          ; ---
        dw      offset fpuWeitek        ; Weitek 1167
        dw      offset fpu87            ; 8087
        dw      offset fpu87W           ; 8087 + Weitek 1167
        dw      offset fpuUnknown       ; ?
        dw      offset fpuUnknown       ; ?
        dw      offset fpu287           ; 80287
        dw      offset fpu287W          ; 80287 + Weitek 1167
        dw      offset fpu2C87          ; Cyrix 2C87
        dw      offset fpu2C87W         ; Cyrix 2C87 + Weitek 1167
        dw      offset fpu387           ; 80387
        dw      offset fpu387W          ; 80387 + Weitek 1167
        dw      offset fpu3C87          ; Cyrix 3C87
        dw      offset fpu3C87W         ; Cyrix 3C87 + Weitek 1167
        dw      offset fpuBuiltIn       ; 486 internal
        dw      offset fpuBuiltInW      ; 486 internal + Weitek 1167
        dw      offset fpu4C87          ; Cyrix 4C87
        dw      offset fpu4C87W         ; Cyrix 4C87 + Weitek 1167
        dw      offset fpu287XL         ; Intel 80287XL
        dw      offset fpu287XLW        ; Intel 80287XL + Weitek 1167
        dw      offset fpuIIT2C87       ; IIT 2C87
        dw      offset fpuIIT2C87W      ; IIT 2C87 + Weitek 1167
        dw      offset fpuIIT3C87       ; IIT 3C87
        dw      offset fpuIIT3C87W      ; IIT 3C87 + Weitek 1167

Lstring FPUis,<Numeric coprocessor: >
Lstring fpuWeitek,<Weitek 1167>
Lstring fpuUnknown,<Unknown>
Lstring fpuNone,<None>
Lstring fpuBuiltIn,<(Built-In)>
Lstring fpu87,<Intel 8087>
Lstring fpu287,<Intel 80287>
Lstring fpu287XL,<Intel 80287XL>
Lstring fpu387,<Intel 80387>
Lstring fpu2C87,<Cyrix 2C87>
Lstring fpu3C87,<Cyrix 3C87>
Lstring fpu4C87,<Cyrix 4C87>
Lstring fpuBuiltInW,<(Built-In) and Weitek 1167>
Lstring fpu87W,<Intel 8087 and Weitek 1167>
Lstring fpu287W,<Intel 80287 and Weitek 1167>
Lstring fpu287XLW,<Intel 80287XL and Weitek 1167>
Lstring fpuIIT2C87,<IIT 2C87>
Lstring fpuIIT2C87W,<IIT 2C87 and Weitek 1167>
Lstring fpuIIT3C87,<IIT 3C87>
Lstring fpuIIT3C87W,<IIT 3C87 and Weitek 1167>
Lstring fpu387W,<Intel 80387 and Weitek 1167>
Lstring fpu2C87W,<Cyrix 2C87 and Weitek 1167>
Lstring fpu3C87W,<Cyrix 3C87 and Weitek 1167>
Lstring fpu4C87W,<Cyrix 4C87 and Weitek 1167>

print_FPU       proc
        .8086
        ldx     FPUis
        call    WriteStr
        mov     bx,FPUType
        shl     bx,1
        mov     dx,FPUs[bx]
        call    WriteStr
        ret
        endp
endif

ifndef  __use_386__
 ifndef __use_286__
        .8086
 endif
endif
