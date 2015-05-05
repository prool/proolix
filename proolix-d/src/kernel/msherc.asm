; MSHerc.com
; Disassembled by Sourcer V 5.04 (C) V COMMUNICATIONS, INC. 1988-1993
; Copyright (C) Comments for Serge Pustovoitoff, 1995

;PAGE  ,78

;ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ
;ÛÛ                                                                      ÛÛ
;ÛÛ                             MSHERC                                   ÛÛ
;ÛÛ                                                                      ÛÛ
;ÛÛ      Created:   7-Mar-88                                             ÛÛ
;ÛÛ      Passes:    9          Analysis Options on: none                 ÛÛ
;ÛÛ                                                                      ÛÛ
;ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ

; The following equates show data references outside the range of the program.

Vec10h         equ     10h*4
equip_bits_     equ     410h
video_mode_     equ     449h
video_columns_  equ     44Ah
video_buf_siz_  equ     44Ch
video_segment_  equ     44Eh
vid_curs_pos0_  equ     450h
vid_curs_pos1_  equ     452h
vid_curs_mode_  equ     460h
video_page_     equ     462h
data_3e         equ     81h
data_143e       equ     9C38h                   ;*
data_144e       equ     0AA39h                  ;*
data_145e       equ     0B053h                  ;*
data_146e       equ     0B83Ah                  ;*
data_147e       equ     2000h
data_148e       equ     7FA6h
data_149e       equ     2000h

seg_a           segment byte public
                assume  cs:seg_a

                org     100h

msherc          proc    far

start:          jmp     StayResident

Trap10h:        push    bp
                push    es
                push    ds
                push    di
                push    si
                push    dx
                push    cx
                push    bx
                push    ax

                mov     bp,sp
                push    cs
                pop     ds
                mov     es,data_25
                cld
                cmp     ah,0EFh
                je      l_Fn_EF ; Fn = EF

; From [Interrupt List]:
;INT 10 - VIDEO - MSHERC.COM - GET VIDEO ADAPTER TYPE AND MODE
;        AH = EFh
;Return: DL = video adapter type
;            00h original Hercules
;            01h Hercules Plus
;            02h Hercules InColor???
;        DH = memory mode byte
;            01h "half" mode
;            03h "full" mode

                or      ah,ah
                jz      l_Fn_0  ; Fn = 00 set video mode
                cmp     byte ptr Flag,0
                jnz     ToOld
                cmp     ah,0Fh
                ja      l_Fn_GT_0F   ; Fn > 0F
l_Fn:          ; Fn <= 0F
                mov     di,[bp+1]
                and     di,0FFh         ; di = ah (from stack)
                shl     di,1            ; di *= 2
                call    word ptr FunTab[di]

l_iret:          ; ¢ëå®¤ ¨§ ¯à¥àë¢ ­¨ï
                pop     ax
                pop     bx
                pop     cx
                pop     dx
                pop     si
                pop     di
                pop     ds
                pop     es
                pop     bp
                iret

l_Fn_GT_0F:     cmp     ah,0F0h
                jb      l_iret   ; Fn > F0 - exit
                jmp     short ToOld

l_Fn_EF:        call    sub_4
                jmp     short l_iret

l_Fn_0:         cmp     al,8
                je      l_Fn
                cmp     al,88h
                je      l_Fn
                mov     byte ptr Flag,1
ToOld:          ; ¥à¥¤ ¥¬ ã¯à. áâ à®¬ã ¢¥ªâ®àã
                pop     ax
                pop     bx
                pop     cx
                pop     dx
                pop     si
                pop     di
                pop     ds
                pop     es
                pop     bp
                jmp     dword ptr cs:OldVec10h

msherc          endp
                public  msherc

;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

Null            proc    near
                retn
Null            endp
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

                db      7 dup (0)
                ; Data table (indexed access)
FunTab          dw      offset SetVideoMode     ; 0 set video mode
data_6          dw      offset SetCur           ; 1 set cursor
data_7          dw      offset SetCurPos        ; 2 set cur pos
data_8          dw      offset GetCurPos        ; 3 get cur pos
data_9          dw      offset Null             ; 4 get pen
data_10         dw      offset SetPage          ; 5 set page
data_11         dw      offset ScrollUp         ; 6 scroll up
data_12         dw      offset ScrollDn         ; 7 scroll down
data_13         dw      offset GetSymAttr       ; 8 get sym/attr
data_14         dw      offset WriteSymAttr     ; 9 write sym/attr
data_15         dw      offset WriteSym         ; A write sym
data_16         dw      offset Null             ; B set palette/border
data_17         dw      offset Dot              ; C dot
data_18         dw      offset GetPoint         ; D get point
data_19         dw      offset tty              ; E tty
data_20         dw      offset GetVideoMode     ; F get video mode

OldVec10h       dd      0
                public  OldVec10h
Flag            db      1
FlagH           db      3       ; or 1 if Flag
data_25         dw      0
data_26         db      0
data_27         db      35h
                db       2Dh, 2Eh, 07h, 5Bh, 02h, 57h
                db       57h, 02h, 03h
                db      0, 0, 0, 0
data_29         dw      0
data_30         dw      0
data_32         dw      0
data_33         db      1                       ; Data table (indexed access)
                db       02h, 04h, 08h, 10h, 20h, 40h
                db       80h
data_34         db      0FEh                    ; Data table (indexed access)
                db      0FCh,0F8h,0F0h,0E0h,0C0h, 80h
                db       00h
data_35         db      0                       ; Data table (indexed access)
                db       01h, 03h, 07h, 0Fh, 1Fh, 3Fh
                db       7Fh
data_36         dw      0                       ; Data table (indexed access)
                db       00h, 20h, 00h, 40h, 00h, 60h
                db       5Ah, 00h, 5Ah, 20h, 5Ah, 40h
                db       5Ah, 60h,0B4h, 00h,0B4h, 20h
                db      0B4h, 40h,0B4h, 60h, 0Eh, 01h
                db       0Eh, 21h, 0Eh, 41h, 0Eh, 61h
                db       68h, 01h, 68h, 21h, 68h, 41h
                db       68h, 61h,0C2h, 01h,0C2h, 21h
                db      0C2h, 41h,0C2h, 61h, 1Ch, 02h
                db       1Ch, 22h, 1Ch, 42h, 1Ch, 62h
                db       76h, 02h, 76h, 22h, 76h, 42h
                db       76h, 62h,0D0h, 02h,0D0h, 22h
                db      0D0h, 42h,0D0h, 62h, 2Ah, 03h
                db       2Ah, 23h, 2Ah, 43h, 2Ah, 63h
                db       84h, 03h, 84h, 23h, 84h, 43h
                db       84h, 63h,0DEh, 03h,0DEh, 23h
                db      0DEh, 43h,0DEh, 63h, 38h, 04h
                db       38h, 24h, 38h, 44h, 38h, 64h
                db       92h, 04h, 92h, 24h, 92h, 44h
                db       92h, 64h,0ECh, 04h,0ECh, 24h
                db      0ECh, 44h,0ECh, 64h, 46h, 05h
                db       46h, 25h, 46h, 45h, 46h, 65h
                db      0A0h, 05h,0A0h, 25h,0A0h, 45h
                db      0A0h, 65h,0FAh, 05h,0FAh, 25h
                db      0FAh, 45h,0FAh, 65h, 54h, 06h
                db       54h, 26h, 54h, 46h, 54h, 66h
                db      0AEh, 06h,0AEh, 26h,0AEh, 46h
                db      0AEh, 66h, 08h, 07h, 08h, 27h
                db       08h, 47h, 08h, 67h, 62h, 07h
                db       62h, 27h, 62h, 47h, 62h, 67h
                db      0BCh, 07h,0BCh, 27h,0BCh, 47h
                db      0BCh, 67h, 16h, 08h, 16h, 28h
                db       16h, 48h, 16h
                db      'hp', 8, 'p(pHph'
                db      0CAh, 08h,0CAh, 28h,0CAh, 48h
                db      0CAh
                db      'h$'
                db      9, '$'
                db      ')$'
                db      'I$'
                db      'i~', 9, '~)~I~i'
                db      0D8h, 09h,0D8h, 29h,0D8h, 49h
                db      0D8h
                db      'i2', 0Ah, '2*2J2j'
                db       8Ch, 0Ah, 8Ch, 2Ah, 8Ch, 4Ah
                db       8Ch, 6Ah,0E6h, 0Ah,0E6h, 2Ah
                db      0E6h, 4Ah,0E6h, 6Ah, 40h, 0Bh
                db       40h, 2Bh, 40h, 4Bh, 40h, 6Bh
                db       9Ah, 0Bh, 9Ah, 2Bh, 9Ah, 4Bh
                db       9Ah, 6Bh,0F4h, 0Bh,0F4h, 2Bh
                db      0F4h, 4Bh,0F4h
                db      'kN', 0Ch, 'N,NLNl'
                db      0A8h, 0Ch,0A8h, 2Ch,0A8h, 4Ch
                db      0A8h, 6Ch, 02h, 0Dh, 02h, 2Dh
                db       02h, 4Dh, 02h
                db      'm\', 0Dh, '\-\M\m'
                db      0B6h, 0Dh,0B6h, 2Dh,0B6h, 4Dh
                db      0B6h, 6Dh, 10h, 0Eh, 10h, 2Eh
                db       10h, 4Eh, 10h, 6Eh, 6Ah, 0Eh
                db       6Ah, 2Eh, 6Ah, 4Eh, 6Ah, 6Eh
                db      0C4h, 0Eh,0C4h, 2Eh,0C4h, 4Eh
                db      0C4h, 6Eh, 1Eh, 0Fh, 1Eh, 2Fh
                db       1Eh, 4Fh, 1Eh, 6Fh, 78h, 0Fh
                db       78h, 2Fh, 78h, 4Fh, 78h, 6Fh
                db      0D2h, 0Fh,0D2h, 2Fh,0D2h, 4Fh
                db      0D2h, 6Fh, 2Ch, 10h, 2Ch, 30h
                db       2Ch, 50h, 2Ch, 70h, 86h, 10h
                db       86h, 30h, 86h, 50h, 86h, 70h
                db      0E0h, 10h,0E0h, 30h,0E0h, 50h
                db      0E0h, 70h, 3Ah, 11h, 3Ah, 31h
                db       3Ah, 51h, 3Ah, 71h, 94h, 11h
                db       94h, 31h, 94h, 51h, 94h, 71h
                db      0EEh, 11h,0EEh, 31h,0EEh, 51h
                db      0EEh, 71h, 48h, 12h, 48h, 32h
                db       48h, 52h, 48h, 72h,0A2h, 12h
                db      0A2h, 32h,0A2h, 52h,0A2h, 72h
                db      0FCh, 12h,0FCh, 32h,0FCh, 52h
                db      0FCh, 72h, 56h, 13h, 56h, 33h
                db       56h, 53h, 56h, 73h,0B0h, 13h
                db      0B0h
loc_7:
                xor     si,ds:data_145e[bx+si]
                jnc     $+0Ch                   ; Jump if carry=0
                adc     al,0Ah
                xor     al,0Ah
loc_8:
                push    sp
                or      dh,[si+64h]
                adc     al,64h                  ; 'd'
                xor     al,64h                  ; 'd'
                push    sp
                db       64h, 74h,0BEh, 14h,0BEh, 34h
                db      0BEh, 54h,0BEh, 74h, 18h, 15h
                db       18h, 35h, 18h, 55h, 18h
                db       75h, 72h
loc_9:
                adc     ax,3572h
;*              jc      loc_16                  ;*Jump if carry Set
                db       72h, 55h               ;  Fixup - byte match
                jc      $+77h                   ; Jump if carry Set
                int     3                       ; Debug breakpoint
                adc     ax,35CCh
                int     3                       ; Debug breakpoint
                push    bp
                int     3                       ; Debug breakpoint
                jnz     loc_14                  ; Jump if not zero
                push    ss
                db       26h, 36h, 26h, 56h, 26h, 76h
                db       80h, 16h, 80h, 36h, 80h, 56h
                db       80h, 76h,0DAh
loc_12:
                push    ss
                db      0DAh, 36h,0DAh, 56h,0DAh, 76h
                db       34h, 17h, 34h, 37h, 34h, 57h
                db       34h, 77h, 8Eh
loc_13:
                pop     ss
                db       8Eh, 37h, 8Eh, 57h, 8Eh
loc_14:
                ja      loc_12                  ; Jump if above
                pop     ss
                call    $-17C6h
                push    di
                call    $+427Ah
                sbb     [bp+si+38h],al
                inc     dx
                pop     ax
                inc     dx
loc_15:
                js      loc_8                   ; Jump if sign=1
                sbb     ds:data_143e[si],bl
                pop     ax
                pushf                           ; Push flags
                js      loc_15                  ; Jump if sign=1
;*              sbb     dh,dh
                db       18h,0F6h               ;  Fixup - byte match
;*              cmp     dh,dh
                db       38h,0F6h               ;  Fixup - byte match
                pop     ax
                idiv    byte ptr [bx+si+50h]    ; al,ah rem = ax/data
                sbb     [bx+si+39h],dx
                push    ax
                pop     cx
                push    ax
;*              jns     loc_10                  ;*Jump if not sign
                db       79h,0AAh               ;  Fixup - byte match
                sbb     ss:data_144e[bp+si],bp
                pop     cx
                stosb                           ; Store al to es:[di]
                jns     loc_17                  ; Jump if not sign
                sbb     al,[si]
                cmp     al,[si]
loc_17:
                pop     dx
                add     al,7Ah                  ; 'z'
                pop     si
                sbb     bl,[bp+3Ah]
                pop     si
                pop     dx
                pop     si
                jp      loc_13                  ; Jump if parity=1
                sbb     bh,ds:data_146e[bx+si]
                pop     dx
                mov     ax,127Ah
                sbb     dx,[bp+si]
                cmp     dx,[bp+si]
                pop     bx
                adc     bh,[bp+di+6Ch]
                sbb     bp,[si+3Bh]
                db       6Ch, 5Bh, 6Ch, 7Bh,0C6h, 1Bh
                db      0C6h, 3Bh,0C6h, 5Bh,0C6h, 7Bh
                db       20h, 1Ch
                db       20h, 3Ch, 20h
data_37         dw      205Ch
                db       7Ch, 7Ah
data_38         dw      7A1Ch
data_39         dw      7A3Ch                   ; Data table (indexed access)
                db       5Ch, 7Ah, 7Ch,0D4h, 1Ch,0D4h
                db       3Ch,0D4h, 5Ch,0D4h, 7Ch, 2Eh
                db       1Dh
                db       2Eh, 3Dh, 2Eh
data_40         db      5Dh
                db       2Eh, 7Dh, 88h, 1Dh, 88h, 3Dh
                db       88h, 5Dh, 88h, 7Dh,0E2h, 1Dh
                db      0E2h, 3Dh,0E2h, 5Dh,0E2h, 7Dh
                db       3Ch, 1Eh, 3Ch, 3Eh, 3Ch, 5Eh
                db       3Ch, 7Eh, 96h, 1Eh, 96h
                db      3Eh
data_41         dw      0
data_43         db      0
data_44         db      0Eh
data_45         db      0
                db      0
                db      25 dup (0)
data_47         db      0
                db      15 dup (0)
                db       7Eh, 81h,0A5h, 81h, 81h,0BDh
                db       99h, 81h, 7Eh, 00h, 00h, 00h
                db       00h, 00h, 7Eh,0FFh,0DBh,0FFh
                db      0FFh,0C3h,0E7h,0FFh, 7Eh, 00h
                db       00h, 00h, 00h, 00h, 00h, 36h
                db       7Fh, 7Fh, 7Fh, 7Fh, 3Eh, 1Ch
                db       08h, 00h, 00h, 00h, 00h, 00h
                db       00h, 08h, 1Ch, 3Eh, 7Fh, 3Eh
                db       1Ch, 08h, 00h, 00h, 00h, 00h
                db       00h, 18h, 3Ch, 3Ch,0E7h,0E7h
                db      0E7h, 3Ch, 18h, 18h, 3Ch, 00h
                db       00h, 00h, 00h, 00h, 18h, 3Ch
                db       7Eh,0FFh,0FFh, 7Eh, 18h, 18h
                db      3Ch
                db      8 dup (0)
                db       18h, 3Ch, 3Ch, 18h, 00h, 00h
                db       00h, 00h, 00h,0FFh,0FFh,0FFh
                db      0FFh,0FFh,0E7h,0C3h, 81h,0E7h
                db      0FFh,0FFh,0FFh,0FFh,0FFh, 00h
                db       00h, 00h, 00h, 3Ch, 66h, 42h
                db       42h, 66h, 3Ch, 00h, 00h, 00h
                db       00h,0FFh,0FFh,0FFh,0FFh,0C3h
                db       99h,0BDh,0BDh, 99h,0C3h,0FFh
                db      0FFh,0FFh,0FFh, 00h, 00h, 0Fh
                db       07h, 0Dh, 19h, 3Ch, 66h, 66h
                db       66h, 3Ch, 00h, 00h, 00h, 00h
                db       00h, 3Ch, 66h, 66h, 66h, 3Ch
                db       18h, 7Eh, 18h, 18h, 00h, 00h
                db       00h, 00h, 00h, 3Fh, 33h, 3Fh
                db       30h, 30h, 30h, 70h,0F0h,0E0h
                db       00h, 00h, 00h, 00h, 00h, 7Fh
                db       63h, 7Fh, 63h, 63h, 63h, 67h
                db      0E7h,0E6h, 60h, 00h, 00h, 00h
                db       00h, 18h, 18h,0DBh, 3Ch,0E7h
                db       3Ch,0DBh, 18h, 18h, 00h, 00h
                db       00h, 00h, 00h, 40h, 60h, 70h
                db       7Ch, 7Fh, 7Ch, 70h, 60h, 40h
                db       00h, 00h, 00h, 00h, 00h, 01h
                db       03h, 07h, 1Fh, 7Fh, 1Fh, 07h
                db       03h, 01h, 00h, 00h, 00h, 00h
                db       00h, 18h, 3Ch, 7Eh, 18h, 18h
                db       18h, 7Eh, 3Ch, 18h, 00h, 00h
                db       00h, 00h, 00h, 33h, 33h, 33h
                db       33h, 33h, 33h, 00h, 33h, 33h
                db       00h, 00h, 00h, 00h, 00h, 7Fh
                db      0DBh,0DBh,0DBh, 7Bh, 1Bh, 1Bh
                db       1Bh, 1Bh, 00h, 00h, 00h, 00h
                db       3Eh, 63h, 30h, 1Ch, 36h, 63h
                db       63h, 36h, 1Ch, 06h
                db       63h, 3Eh
                db      9 dup (0)
                db       7Fh, 7Fh, 7Fh, 00h, 00h, 00h
                db       00h, 00h, 18h, 3Ch, 7Eh, 18h
                db       18h, 18h, 7Eh, 3Ch, 18h, 00h
                db       00h, 00h, 00h, 18h, 3Ch, 7Eh
                db       18h, 18h, 18h, 18h, 18h, 18h
                db       00h, 00h, 00h, 00h, 00h, 00h
                db       18h, 18h, 18h, 18h, 18h, 18h
                db       7Eh, 3Ch, 18h, 00h, 00h, 00h
                db       00h, 00h, 00h, 00h, 0Ch, 06h
                db      0FFh, 06h, 0Ch, 00h
                db      8 dup (0)
                db       18h, 30h, 7Fh, 30h, 18h, 00h
                db      9 dup (0)
                db       60h, 60h, 60h, 7Fh, 00h
                db      8 dup (0)
                db       24h, 66h,0FFh, 66h, 24h
                db      8 dup (0)
                db       08h, 1Ch, 1Ch, 3Eh, 3Eh, 7Fh
                db       7Fh
                db      7 dup (0)
                db       7Fh, 7Fh, 3Eh, 3Eh, 1Ch, 1Ch
                db       08h, 00h
                db      19 dup (0)
                db       18h, 3Ch, 3Ch, 3Ch, 18h, 18h
                db       00h, 18h, 18h, 00h, 00h, 00h
                db       00h, 63h, 63h, 63h, 22h, 00h
                db      10 dup (0)
                db       36h, 36h, 7Fh, 36h, 36h, 36h
                db       7Fh, 36h, 36h, 00h, 00h, 00h
                db       0Ch, 0Ch, 3Eh, 63h, 61h, 60h
                db       3Eh, 03h, 43h, 63h, 3Eh, 0Ch
                db       0Ch, 00h, 00h, 00h, 00h, 00h
                db       61h, 63h, 06h, 0Ch, 18h, 33h
                db       63h, 00h, 00h, 00h, 00h, 00h
                db       1Ch, 36h, 36h, 1Ch, 3Bh, 6Eh
                db       66h, 66h, 3Bh, 00h, 00h, 00h
                db       00h
                db       30h, 30h, 30h, 60h
                db      11 dup (0)
                db       0Ch, 18h, 30h, 30h, 30h, 30h
                db       30h, 18h, 0Ch, 00h, 00h, 00h
                db       00h, 00h, 18h, 0Ch, 06h, 06h
                db       06h, 06h, 06h, 0Ch, 18h
                db      7 dup (0)
                db       66h, 3Ch,0FFh, 3Ch, 66h
                db      8 dup (0)
                db       18h, 18h, 18h,0FFh, 18h, 18h
                db       18h, 00h
                db      11 dup (0)
                db       18h, 18h, 18h, 30h
                db      9 dup (0)
                db      0FFh, 00h
                db      14 dup (0)
                db       18h, 18h, 00h, 00h, 00h, 00h
                db       00h, 01h, 03h, 06h, 0Ch, 18h
                db       30h, 60h,0C0h, 80h, 00h, 00h
                db       00h, 00h, 00h
                db      '>cgo{scc>'
                db       00h, 00h, 00h, 00h, 00h, 0Ch
                db       1Ch, 3Ch, 0Ch, 0Ch, 0Ch, 0Ch
                db       0Ch, 3Fh, 00h, 00h, 00h, 00h
                db       00h, 3Eh, 63h, 03h, 06h, 0Ch
                db       18h, 30h, 63h, 7Fh, 00h, 00h
                db       00h, 00h, 00h, 3Eh, 63h, 03h
                db       03h, 1Eh, 03h, 03h, 63h, 3Eh
                db       00h, 00h, 00h, 00h, 00h, 06h
                db       0Eh, 1Eh, 36h, 66h, 7Fh, 06h
                db       06h, 0Fh, 00h, 00h, 00h, 00h
                db       00h, 7Fh, 60h, 60h, 60h, 7Eh
                db       03h, 03h, 63h, 3Eh, 00h, 00h
                db       00h, 00h, 00h, 1Ch
                db      '0``~ccc>'
                db       00h, 00h, 00h, 00h, 00h, 7Fh
                db       63h, 03h, 06h, 0Ch, 18h, 18h
                db       18h, 18h, 00h, 00h, 00h, 00h
                db       00h
                db      '>ccc>ccc>'
                db       00h, 00h, 00h, 00h, 00h, 3Eh
                db       63h, 63h, 63h, 3Fh, 03h, 03h
                db       06h, 3Ch, 00h, 00h, 00h, 00h
                db       00h, 00h, 18h, 18h, 00h, 00h
                db       18h, 18h, 00h
                db      7 dup (0)
                db       18h, 18h, 00h, 00h, 00h, 18h
                db       18h, 30h, 00h, 00h, 00h, 00h
                db       00h, 06h, 0Ch, 18h, 30h, 60h
                db       30h, 18h, 0Ch, 06h
                db      8 dup (0)
                db       7Eh, 00h, 00h, 7Eh, 00h, 00h
                db       00h, 00h, 00h, 00h, 00h, 60h
                db       30h, 18h, 0Ch, 06h, 0Ch, 18h
                db       30h, 60h, 00h, 00h, 00h, 00h
                db       00h, 3Eh, 63h, 63h, 06h, 0Ch
                db       0Ch, 00h, 0Ch, 0Ch, 00h, 00h
                db       00h, 00h, 00h
                db      '>ccooon`>'
                db       00h, 00h, 00h, 00h, 00h, 08h
                db       1Ch, 36h, 63h, 63h, 7Fh, 63h
                db       63h, 63h, 00h, 00h, 00h, 00h
                db       00h
                db      '~333>333~'
                db       00h, 00h, 00h, 00h, 00h, 1Eh
                db       33h, 61h, 60h, 60h, 60h, 61h
                db       33h, 1Eh, 00h, 00h, 00h, 00h
                db       00h
                db      '|6333336|'
                db       00h, 00h, 00h, 00h, 00h, 7Fh
                db       33h, 31h, 34h, 3Ch, 34h, 31h
                db       33h, 7Fh, 00h, 00h, 00h, 00h
                db       00h, 7Fh
                db      '314<400x'
                db       00h, 00h, 00h, 00h, 00h, 1Eh
                db       33h, 61h, 60h, 60h, 6Fh, 63h
                db       33h, 1Dh, 00h, 00h, 00h, 00h
                db       00h, 63h, 63h, 63h, 63h, 7Fh
                db       63h, 63h, 63h, 63h, 00h, 00h
                db       00h, 00h, 00h
                db      3Ch
                db      7 dup (18h)
                db       3Ch, 00h, 00h, 00h, 00h, 00h
                db       0Fh, 06h, 06h, 06h, 06h, 06h
                db       66h, 66h, 3Ch, 00h, 00h, 00h
                db       00h, 00h
                db      's366<663s'
                db      0, 0, 0, 0, 0
                db      'x0000013'
                db       7Fh, 00h, 00h, 00h, 00h, 00h
                db      0C3h,0E7h,0FFh,0DBh,0C3h,0C3h
                db      0C3h,0C3h,0C3h, 00h, 00h, 00h
                db       00h, 00h, 63h, 73h, 7Bh, 7Fh
                db       6Fh, 67h, 63h, 63h, 63h, 00h
                db       00h, 00h, 00h, 00h, 1Ch, 36h
                db       63h, 63h, 63h, 63h, 63h, 36h
                db       1Ch, 00h, 00h, 00h, 00h, 00h
                db      '~333>000x'
                db      0, 0, 0, 0, 0
                db      '>ccccko>'
                db      6, 7, 0, 0, 0, 0
                db      '~333>633{'
                db       00h, 00h, 00h, 00h, 00h, 3Eh
                db       63h, 63h, 30h, 1Ch, 06h, 63h
                db       63h, 3Eh, 00h, 00h, 00h, 00h
                db       00h,0FFh,0DBh, 99h, 18h, 18h
                db       18h, 18h, 18h, 3Ch, 00h, 00h
                db       00h, 00h, 00h
                db      'cccccccc>'
                db       00h, 00h, 00h, 00h, 00h,0C3h
                db      0C3h,0C3h,0C3h,0C3h,0C3h, 66h
                db       3Ch, 18h, 00h, 00h, 00h, 00h
                db       00h,0C3h,0C3h,0C3h,0C3h,0C3h
                db      0DBh,0FFh, 66h, 66h, 00h, 00h
                db       00h, 00h, 00h,0C3h,0C3h, 66h
                db       3Ch, 18h, 3Ch, 66h,0C3h,0C3h
                db       00h, 00h, 00h, 00h, 00h,0C3h
                db      0C3h,0C3h, 66h, 3Ch, 18h, 18h
                db       18h, 3Ch, 00h, 00h, 00h, 00h
                db       00h,0FFh,0C3h, 86h, 0Ch, 18h
                db       30h, 61h,0C3h,0FFh, 00h, 00h
                db       00h, 00h, 00h
                db      3Ch
                db      7 dup (30h)
                db       3Ch, 00h, 00h, 00h, 00h, 00h
                db       40h, 60h, 70h, 38h, 1Ch, 0Eh
                db       07h, 03h, 01h, 00h, 00h, 00h
                db       00h, 00h
                db      3Ch
                db      7 dup (0Ch)
                db       3Ch, 00h, 00h, 00h, 08h, 1Ch
                db       36h, 63h
                db      21 dup (0)
                db      0FFh, 00h, 00h, 18h, 18h, 0Ch
                db      16 dup (0)
                db       3Ch, 06h, 3Eh, 66h, 66h, 3Bh
                db       00h, 00h, 00h, 00h, 00h
                db      'p00<6333n'
                db      0
                db      7 dup (0)
                db       3Eh, 63h, 60h, 60h, 63h, 3Eh
                db       00h, 00h, 00h, 00h, 00h, 0Eh
                db       06h, 06h, 1Eh, 36h, 66h, 66h
                db       66h, 3Bh, 00h
                db      7 dup (0)
                db       3Eh, 63h, 7Fh, 60h, 63h, 3Eh
                db       00h, 00h, 00h, 00h, 00h, 1Ch
                db      '620|000x'
                db      0
                db      7 dup (0)
                db       3Bh, 66h, 66h, 66h, 3Eh, 06h
                db       66h, 3Ch, 00h, 00h, 00h
                db      'p006;333s'
                db       00h, 00h, 00h, 00h, 00h, 0Ch
                db       0Ch, 00h, 1Ch, 0Ch, 0Ch, 0Ch
                db       0Ch, 1Eh, 00h, 00h, 00h, 00h
                db       00h, 06h, 06h, 00h, 0Eh, 06h
                db       06h, 06h, 66h, 66h, 3Ch, 00h
                db       00h, 00h, 00h
                db      'p0036<63s'
                db       00h, 00h, 00h, 00h, 00h, 1Ch
                db      7 dup (0Ch)
                db      1Eh
                db      8 dup (0)
                db      0E6h,0DBh,0DBh,0DBh,0DBh,0DBh
                db      8 dup (0)
                db       6Eh, 33h, 33h, 33h, 33h, 33h
                db       00h
                db      7 dup (0)
                db       3Eh, 63h, 63h, 63h, 63h, 3Eh
                db      8 dup (0)
                db      'n333>00x'
                db       00h, 00h, 00h, 00h, 00h, 00h
                db       3Bh, 66h, 66h, 66h, 3Eh, 06h
                db       06h, 0Fh, 00h, 00h, 00h, 00h
                db       00h, 00h, 6Eh, 33h, 33h, 30h
                db       30h, 78h, 00h
                db      7 dup (0)
                db       3Eh, 63h, 38h, 0Eh, 63h, 3Eh
                db       00h, 00h, 00h, 00h, 00h, 08h
                db       18h, 18h, 7Eh, 18h, 18h, 18h
                db       1Bh, 0Eh, 00h, 00h, 00h, 00h
                db       00h, 00h, 00h, 00h, 66h, 66h
                db       66h, 66h, 66h, 3Bh, 00h
                db      7 dup (0)
                db      0C3h,0C3h,0C3h, 66h, 3Ch, 18h
                db      8 dup (0)
                db      0C3h,0C3h,0DBh,0DBh, 7Eh, 66h
                db      8 dup (0)
                db       63h, 36h, 1Ch, 1Ch, 36h, 63h
                db      8 dup (0)
                db       63h, 63h, 63h, 63h, 3Fh, 03h
                db       06h, 3Ch, 00h, 00h, 00h, 00h
                db       00h, 00h, 7Fh, 66h, 0Ch, 18h
                db       33h, 7Fh, 00h, 00h, 00h, 00h
                db       00h, 0Eh, 18h, 18h, 18h, 70h
                db       18h, 18h, 18h, 0Eh, 00h, 00h
                db       00h, 00h, 00h, 18h, 18h, 18h
                db       18h, 00h, 18h, 18h, 18h, 18h
                db       00h, 00h, 00h, 00h, 00h, 70h
                db       18h, 18h, 18h, 1Ch, 18h, 18h
                db       18h, 70h, 00h, 00h, 00h, 00h
                db       00h
                db       3Bh, 6Eh
                db      15 dup (0)
                db       08h, 1Ch, 36h, 63h, 63h, 63h
                db       7Fh, 00h, 00h, 00h, 00h, 1Eh
                db       33h, 61h, 60h, 60h, 61h, 63h
                db       3Eh, 06h, 03h, 3Eh, 00h, 00h
                db       00h, 66h, 66h, 00h, 66h, 66h
                db       66h, 66h, 66h, 3Bh, 00h, 00h
                db       00h, 00h, 06h, 0Ch, 18h, 00h
                db       3Eh, 63h, 7Fh, 60h, 63h, 3Eh
                db       00h, 00h, 00h, 00h, 08h, 1Ch
                db       36h, 00h, 3Ch, 06h, 3Eh, 66h
                db       66h, 3Bh, 00h, 00h, 00h, 00h
                db       00h, 66h, 66h, 00h, 3Ch, 06h
                db       3Eh, 66h, 66h, 3Bh, 00h, 00h
                db       00h, 00h, 30h, 18h, 0Ch, 00h
                db       3Ch, 06h, 3Eh, 66h, 66h, 3Bh
                db       00h, 00h, 00h, 00h, 1Ch, 36h
                db       1Ch, 00h, 3Ch, 06h, 3Eh, 66h
                db       66h, 3Bh, 00h
                db      7 dup (0)
                db       3Ch, 66h, 60h, 66h, 3Ch, 06h
                db       3Ch, 00h, 00h, 00h, 08h, 1Ch
                db       36h, 00h, 3Eh, 63h, 7Fh, 60h
                db       63h, 3Eh, 00h, 00h, 00h, 00h
                db       00h, 66h, 66h, 00h, 3Eh, 63h
                db       7Fh, 60h, 63h, 3Eh, 00h, 00h
                db       00h, 00h, 30h, 18h, 0Ch, 00h
                db       3Eh, 63h, 7Fh, 60h, 63h, 3Eh
                db       00h, 00h, 00h, 00h, 00h, 36h
                db       36h, 00h, 1Ch, 0Ch, 0Ch, 0Ch
                db       0Ch, 1Eh, 00h, 00h, 00h, 00h
                db       18h, 3Ch, 66h, 00h, 38h, 18h
                db       18h, 18h, 18h, 3Ch, 00h, 00h
                db       00h, 00h, 60h, 30h, 18h, 00h
                db       38h, 18h, 18h, 18h, 18h, 3Ch
                db       00h, 00h, 00h, 00h, 63h, 63h
                db       08h, 1Ch, 36h, 63h, 63h, 7Fh
                db       63h, 63h, 00h, 00h, 00h, 1Ch
                db       36h, 1Ch, 00h, 1Ch, 36h, 63h
                db       63h, 7Fh, 63h, 63h, 00h, 00h
                db       00h, 0Ch, 18h, 30h, 00h, 7Fh
                db       33h, 30h, 3Eh, 30h, 33h, 7Fh
                db       00h
                db      7 dup (0)
                db       6Eh, 3Bh, 1Bh, 7Eh,0D8h,0DCh
                db       77h, 00h, 00h, 00h, 00h, 1Fh
                db       36h, 66h, 66h, 7Fh, 66h, 66h
                db       66h, 67h, 00h, 00h, 00h, 00h
                db       08h, 1Ch, 36h, 00h, 3Eh, 63h
                db       63h, 63h, 63h, 3Eh, 00h, 00h
                db       00h, 00h, 00h, 63h, 63h, 00h
                db       3Eh, 63h, 63h, 63h, 63h, 3Eh
                db       00h, 00h, 00h, 00h, 30h, 18h
                db       0Ch, 00h, 3Eh, 63h, 63h, 63h
                db       63h, 3Eh, 00h, 00h, 00h, 00h
                db       18h, 3Ch, 66h, 00h, 66h, 66h
                db       66h, 66h, 66h, 3Bh, 00h, 00h
                db       00h, 00h, 30h, 18h, 0Ch, 00h
                db       66h, 66h, 66h, 66h, 66h, 3Bh
                db       00h, 00h, 00h, 00h, 00h, 63h
                db       63h, 00h, 63h, 63h, 63h, 63h
                db       3Fh, 03h, 06h, 3Ch, 00h, 00h
                db       63h, 63h, 1Ch, 36h, 63h, 63h
                db       63h, 63h, 36h, 1Ch, 00h, 00h
                db       00h, 00h, 63h, 63h, 00h, 63h
                db       63h, 63h, 63h, 63h, 63h, 3Eh
                db       00h, 00h, 00h, 00h, 18h, 18h
                db       7Eh,0C3h,0C0h,0C0h,0C3h, 7Eh
                db       18h, 18h, 00h, 00h, 00h, 00h
                db       1Ch
                db      '600x000s~'
                db       00h, 00h, 00h, 00h, 00h,0C3h
                db       66h, 3Ch, 18h,0FFh, 18h,0FFh
                db       18h, 18h, 00h, 00h, 00h, 00h
                db      0FCh
                db      'ff|bfoff'
                db      0F3h, 00h, 00h, 00h, 00h, 0Eh
                db       1Bh, 18h, 18h, 18h, 7Eh, 18h
                db       18h, 18h, 18h,0D8h, 70h, 00h
                db       00h, 0Ch, 18h, 30h, 00h, 3Ch
                db       06h, 3Eh, 66h, 66h, 3Bh, 00h
                db       00h, 00h, 00h, 0Ch, 18h, 30h
                db       00h, 38h, 18h, 18h, 18h, 18h
                db       3Ch, 00h, 00h, 00h, 00h, 0Ch
                db       18h, 30h, 00h, 3Eh, 63h, 63h
                db       63h, 63h, 3Eh, 00h, 00h, 00h
                db       00h, 0Ch, 18h, 30h, 00h, 66h
                db       66h, 66h, 66h, 66h, 3Bh, 00h
                db       00h, 00h, 00h, 00h, 3Bh, 6Eh
                db       00h, 6Eh, 33h, 33h, 3Bh, 33h
                db       33h, 33h, 00h, 00h, 3Bh, 6Eh
                db       00h, 63h, 73h, 7Bh, 7Fh, 6Fh
                db       67h, 63h, 63h, 00h, 00h, 00h
                db       00h, 3Ch, 6Ch, 6Ch, 3Eh, 00h
                db       7Eh, 00h
                db      7 dup (0)
                db       38h, 6Ch, 6Ch, 38h, 00h, 7Ch
                db       00h
                db      8 dup (0)
                db       18h, 18h, 00h, 18h, 18h, 33h
                db       33h, 1Eh, 00h, 00h
                db      8 dup (0)
                db       7Fh, 60h, 60h
                db      11 dup (0)
                db       7Fh, 03h, 03h, 00h, 00h, 00h
                db       00h, 00h, 00h, 60h,0E0h, 63h
                db       66h, 6Ch, 18h, 30h, 6Eh,0C3h
                db       06h, 0Ch, 1Fh, 00h, 00h, 60h
                db      0E0h, 63h, 66h, 6Ch, 18h, 30h
                db       67h,0CFh, 1Fh, 03h, 03h, 00h
                db       00h, 00h, 18h, 18h, 00h, 18h
                db       18h, 3Ch, 3Ch, 3Ch, 18h, 00h
                db       00h, 00h, 00h, 00h, 00h, 00h
                db       1Bh, 36h, 6Ch, 36h, 1Bh, 00h
                db      8 dup (0)
                db       6Ch, 36h, 1Bh, 36h, 6Ch, 00h
                db       00h, 00h, 00h, 00h, 11h, 44h
                db       11h, 44h, 11h, 44h, 11h, 44h
                db       11h, 44h, 11h, 44h, 11h, 44h
                db       55h,0AAh, 55h,0AAh, 55h,0AAh
                db       55h,0AAh, 55h,0AAh, 55h,0AAh
                db       55h,0AAh,0DDh, 77h,0DDh, 77h
                db      0DDh, 77h,0DDh, 77h,0DDh, 77h
                db      0DDh, 77h,0DDh, 77h, 18h
                db      20 dup (18h)
                db      0F8h
                db      11 dup (18h)
                db      0F8h, 18h,0F8h, 18h, 18h, 18h
                db       18h, 18h, 18h
                db      7 dup (36h)
                db      0F6h, 36h, 36h, 36h, 36h, 36h
                db       36h, 00h, 00h, 00h, 00h, 00h
                db       00h,0FEh
                db      7 dup (36h)
                db       00h, 00h, 00h, 00h, 00h,0F8h
                db       18h,0F8h, 18h, 18h, 18h, 18h
                db       18h, 18h, 36h, 36h, 36h, 36h
                db       36h,0F6h, 06h,0F6h
                db       36h, 36h
                db      18 dup (36h)
                db       00h, 00h, 00h, 00h, 00h,0FEh
                db       06h,0F6h
                db       36h, 36h, 36h
                db      8 dup (36h)
                db      0F6h, 06h,0FEh, 00h, 00h, 00h
                db       00h, 00h, 00h, 36h, 36h, 36h
                db       36h, 36h, 36h, 36h,0FEh, 00h
                db       00h, 00h, 00h, 00h, 00h, 18h
                db       18h, 18h, 18h, 18h,0F8h, 18h
                db      0F8h
                db      13 dup (0)
                db      0F8h
                db      13 dup (18h)
                db       1Fh, 00h, 00h, 00h, 00h, 00h
                db       00h, 18h, 18h, 18h, 18h, 18h
                db       18h, 18h,0FFh, 00h
                db      12 dup (0)
                db      0FFh, 18h
                db      12 dup (18h)
                db       1Fh, 18h, 18h, 18h, 18h, 18h
                db       18h
                db      7 dup (0)
                db      0FFh, 00h, 00h, 00h, 00h, 00h
                db       00h, 18h, 18h, 18h, 18h, 18h
                db       18h, 18h,0FFh, 18h
                db      10 dup (18h)
                db       1Fh, 18h, 1Fh, 18h, 18h, 18h
                db       18h, 18h, 18h
                db      7 dup (36h)
                db      37h
                db      11 dup (36h)
                db       37h, 30h, 3Fh
                db      11 dup (0)
                db       3Fh, 30h, 37h
                db      11 dup (36h)
                db      0F7h, 00h,0FFh, 00h
                db      10 dup (0)
                db      0FFh, 00h,0F7h, 36h, 36h, 36h
                db      8 dup (36h)
                db      '707666666'
                db       00h, 00h, 00h, 00h, 00h,0FFh
                db       00h,0FFh, 00h, 00h, 00h, 00h
                db       00h, 00h, 36h, 36h, 36h, 36h
                db       36h,0F7h, 00h,0F7h, 36h, 36h
                db       36h, 36h, 36h, 36h, 18h, 18h
                db       18h, 18h, 18h,0FFh, 00h,0FFh
                db       00h, 00h, 00h, 00h, 00h, 00h
                db       36h, 36h, 36h, 36h, 36h, 36h
                db       36h,0FFh, 00h
                db      10 dup (0)
                db      0FFh, 00h,0FFh, 18h, 18h, 18h
                db       18h, 18h, 18h, 00h, 00h, 00h
                db       00h, 00h, 00h, 00h,0FFh
                db      13 dup (36h)
                db       3Fh, 00h, 00h, 00h, 00h, 00h
                db       00h, 18h, 18h, 18h, 18h, 18h
                db       1Fh, 18h, 1Fh
                db      11 dup (0)
                db       1Fh, 18h, 1Fh, 18h, 18h, 18h
                db       18h, 18h, 18h
                db      7 dup (0)
                db      3Fh
                db      13 dup (36h)
                db      0FFh, 36h, 36h, 36h, 36h, 36h
                db       36h, 18h, 18h, 18h, 18h, 18h
                db      0FFh, 18h,0FFh
                db      13 dup (18h)
                db      0F8h
                db      13 dup (0)
                db       1Fh, 18h, 18h, 18h, 18h, 18h
                db       18h
                db      14 dup (0FFh)
                db      7 dup (0)
                db      7 dup (0FFh)
                db      14 dup (0F0h)
                db      14 dup (0Fh)
                db      7 dup (0FFh)
                db      12 dup (0)
                db       3Bh, 6Eh, 6Ch, 6Ch, 6Eh, 3Bh
                db       00h, 00h, 00h, 00h, 00h, 00h
                db       00h
                db      '>c~cc~`` '
                db       00h, 00h, 00h, 7Fh
                db      'cc``````'
                db      7 dup (0)
                db       7Fh, 36h, 36h, 36h, 36h, 36h
                db       36h, 00h, 00h, 00h, 00h, 00h
                db       7Fh, 63h, 30h, 18h, 0Ch, 18h
                db       30h, 63h, 7Fh
                db      8 dup (0)
                db       3Eh, 6Ch, 6Ch, 6Ch, 6Ch, 38h
                db       00h, 00h, 00h, 00h, 00h, 00h
                db       00h
                db      '3333>00`'
                db       00h, 00h, 00h, 00h, 00h, 00h
                db       3Bh, 6Eh, 0Ch, 0Ch, 0Ch, 0Ch
                db       0Ch, 00h, 00h, 00h, 00h, 00h
                db       7Eh, 18h, 3Ch, 66h, 66h, 66h
                db       3Ch, 18h, 7Eh, 00h, 00h, 00h
                db       00h, 00h, 1Ch, 36h, 63h, 63h
                db       7Fh, 63h, 63h, 36h, 1Ch, 00h
                db       00h, 00h, 00h, 00h, 1Ch, 36h
                db       63h, 63h, 63h, 36h, 1Ch, 36h
                db       36h, 77h, 00h, 00h, 00h, 00h
                db       1Eh, 30h, 18h, 0Ch, 3Eh, 66h
                db       66h, 66h, 3Ch, 00h
                db      7 dup (0)
                db       7Eh,0DBh,0DBh, 7Eh, 00h, 00h
                db       00h, 00h, 00h, 00h, 00h, 03h
                db       06h, 7Eh,0DBh,0DBh,0F3h, 7Eh
                db       60h,0C0h, 00h, 00h, 00h, 00h
                db       00h, 1Ch, 30h, 60h, 60h, 7Ch
                db       60h, 60h, 30h, 1Ch
                db      7 dup (0)
                db      3Eh
                db      7 dup (63h)
                db       00h, 00h, 00h, 00h, 00h, 00h
                db       7Fh, 00h, 7Fh, 7Fh, 00h, 7Fh
                db       00h, 00h, 00h, 00h, 00h, 00h
                db       18h, 18h, 18h,0FFh, 18h, 18h
                db       18h, 00h,0FFh, 00h, 00h, 00h
                db       00h, 00h, 30h, 18h, 0Ch, 06h
                db       0Ch, 18h, 30h, 00h,0FFh, 00h
                db       00h, 00h, 00h, 00h, 0Ch, 18h
                db       30h, 60h, 30h, 18h, 0Ch, 00h
                db      0FFh, 00h, 00h, 00h, 00h, 00h
                db       0Eh
                db       1Bh, 1Bh
                db      17 dup (18h)
                db      0D8h,0D8h, 70h, 00h, 00h, 00h
                db       00h, 00h, 18h, 18h, 00h, 00h
                db      0FFh, 00h, 00h, 18h, 18h
                db      7 dup (0)
                db       3Bh, 6Eh, 00h, 3Bh, 6Eh, 00h
                db       00h, 00h, 00h, 00h, 00h, 38h
                db       6Ch, 6Ch, 38h, 00h
                db      14 dup (0)
                db       18h, 18h
                db      13 dup (0)
                db       30h, 00h, 00h, 00h, 00h, 00h
                db       00h, 00h, 0Fh, 0Ch, 0Ch, 0Ch
                db       0Ch, 0Ch,0ECh, 6Ch, 3Ch, 1Ch
                db       00h, 00h, 00h, 00h,0D8h
                db       6Ch, 6Ch, 6Ch, 6Ch, 6Ch
                db      8 dup (0)
                db       70h,0D8h, 30h, 60h,0C8h,0F8h
                db      11 dup (0)
                db       3Ch, 3Ch, 3Ch, 3Ch, 3Ch, 3Ch
                db      18 dup (0)

;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

GetVideoMode    proc    near
                mov     ah,es:video_columns_
                mov     al,8
                mov     [bp],ax
                mov     al,es:video_page_
                mov     [bp+3],al
                retn
GetVideoMode    endp

;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

SetVideoMode    proc    near
                mov     ah,es:equip_bits_
                and     ah,30h
                cmp     ah,30h
                je      loc_20
                retn
loc_20:
                mov     data_26,2
                push    ax
                xor     ax,ax
                mov     byte ptr es:video_mode_,8
                mov     word ptr es:video_columns_,50h
                mov     es:video_page_,al
                mov     es:vid_curs_pos0_,ax
                cmp     FlagH,3
                jne     loc_21                  ; Jump if not equal
                mov     es:vid_curs_pos1_,ax
loc_21:
                mov     es:video_segment_,ax
                mov     word ptr es:video_buf_siz_,8000h
                mov     Flag,al
                mov     si,offset data_27
                mov     cx,0Eh
                mov     ah,0
                cli                             ; Disable interrupts
                mov     dx,3B8h
                mov     al,data_26
                out     dx,al                   ; port 3B8h, MDA video control
                call    sub_6
                mov     dx,3BFh
                mov     al,FlagH
                out     dx,al                   ; port 3BFh, Hercules config
                mov     cx,14h
                call    sub_19
                pop     ax
                test    al,80h
                jnz     loc_23                  ; Jump if not zero
                mov     ax,0B000h
                push    es
                mov     es,ax
                xor     di,di                   ; Zero register
                xor     ax,ax                   ; Zero register
                mov     cx,8000h
                cmp     FlagH,3
                je      loc_22                  ; Jump if equal
                shr     cx,1                    ; Shift w/zeros fill
loc_22:
                rep     stosw                   ; Rep when cx >0 Store ax to es:[di]
                pop     es
loc_23:
                mov     dx,3B8h
                mov     al,data_26
                or      al,8
                mov     data_26,al
                out     dx,al                   ; port 3B8h, MDA video control
                retn
SetVideoMode    endp


;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

sub_4           proc    near
                call    TestHerc
                mov     [bp+6],dx
                retn
sub_4           endp

;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

TestHerc        proc    near    ; ¯à®¢¥àª  ­  ­ «¨ç¨¥ ƒ¥àªã«¥á 
; ‚ëå®¤: dl =   ff - ­¥ ƒ¥àªã«¥á
;        dl =   02 - Hercules in Color
;        dl =   01 - Hercules+
;        dl =   00 - Hercules
                mov     dx,3BAh
                in      al,dx   ; port 3BAh, MDA/EGA vid status, 6845 chip
                and     al,70h  ; 0111 0000 ¢ë¤¥«¥­¨¥ ¡¨â®¢ â¨¯  ƒ¥àªã«¥á 
                                ; (see [Interrupt List])
                                ;  000 = adapter is Hercules or compatible
                                ;  001 = adapter is Hercules+
                                ;  101 = adapter is Hercules InColor
                                ;        else: adapter is unknown
                cmp     al,50h  ; 0101 0000 (Hercules InColor)
                jne     @@24
                mov     dl,2
                jmp     short @@ret
                db      90h
@@24:
                cmp     al,10h
                jne     @@25
                mov     dl,1
                jmp     short @@ret
                db      90h
@@25:         ; à®¢¥àª  ­  ¬¨£ ­¨¥ áâ àè¥£® ¡¨â  ¯®àâ  3BA
                mov     dx,3BAh
                mov     cx,0FFFFh
                in      al,dx   ; port 3BAh, MDA/EGA vid status
                mov     bl,al

@@loop:
                in      al,dx   ; port 3BAh, MDA/EGA vid status
                xor     al,bl
                test    al,80h
                jnz     @@27  ; ¡¨â ¬¨£ ¥â
                loop    @@loop
                ; ¤  ¢à®¤¥ ­¥ ¬¨£ ¥â
                ; ergo - íâ® ­¥ ƒ¥àªã«¥á

                mov     dl,0FFh
                jmp     short @@ret
@@27:         ; ‡­ ç¨â, ¯à®áâ® ƒ¥ªã«¥á ¨«¨ çâ®-â® á®¢¬¥áâ¨¬®¥
                xor     dl,dl
@@ret:
                mov     dh,FlagH
                shr     dh,1
                retn
TestHerc        endp
                public  TestHerc

;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

sub_6           proc    near
                cli                             ; Disable interrupts

locloop_29:
                mov     dx,3B4h
                mov     al,ah
                out     dx,al                   ; port 3B4h, MDA/EGA reg index
                                                ;  al = 0, horiz char total
                inc     dx
                lodsb                           ; String [si] to al
                out     dx,al                   ; port 3B5h, MDA/EGA indxd data
                inc     ah
                loop    locloop_29              ; Loop if cx > 0

                sti                             ; Enable interrupts
                retn
sub_6           endp

                db      0

;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

SetPage           proc    near
                mov     ah,data_26
                and     ah,7Fh
                xor     cx,cx                   ; Zero register
                and     al,1
                jz      loc_30                  ; Jump if zero
                dec     al
                cmp     FlagH,3
                jne     loc_30                  ; Jump if not equal
                inc     al
                mov     cx,8000h
                or      ah,80h
loc_30:
                mov     data_26,ah
                mov     es:video_page_,al
                mov     es:video_segment_,cx
                push    ax
                mov     dx,3B8h
                mov     al,ah
                out     dx,al                   ; port 3B8h, MDA video control
                pop     ax
                xor     ah,ah                   ; Zero register
                shl     ax,1                    ; Shift w/zeros fill
                mov     di,ax
                mov     bx,es:vid_curs_pos0_[di]
                retn
SetPage           endp


;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

SetCurPos           proc    near
                xor     ax,ax                   ; Zero register
                mov     al,bh
                and     ax,1
                shl     ax,1                    ; Shift w/zeros fill
                mov     di,ax
                mov     es:vid_curs_pos0_[di],dx
                retn
SetCurPos           endp


;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

GetCurPos           proc    near
                xor     ax,ax                   ; Zero register
                mov     al,bh
                shl     ax,1                    ; Shift w/zeros fill
                mov     di,ax
                mov     dx,es:vid_curs_pos0_[di]
                mov     [bp+6],dx
                mov     cx,es:vid_curs_mode_
                mov     [bp+4],cx
                retn
GetCurPos           endp


;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

SetCur          proc    near
                mov     es:vid_curs_mode_,cx
                retn
SetCur          endp

                db      0

;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

WriteSym          proc    near
                mov     bh,1
                jmp     loc_44

;ßßßß External Entry into Subroutine ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

GetSymAttr:
                mov     bl,bh
                xor     bh,bh                   ; Zero register
                mov     cx,0B800h
                or      bl,bl                   ; Zero ?
                jnz     loc_31                  ; Jump if not zero
                mov     cx,0B000h
                push    cx
loc_31:
                shl     bx,1                    ; Shift w/zeros fill
                mov     bx,es:vid_curs_pos0_[bx]
                mov     al,bh
                xor     ah,ah                   ; Zero register
                mul     data_44                 ; ax = data * al
                push    ax
                xor     bh,bh                   ; Zero register
                mov     cx,bx
                shl     bx,1                    ; Shift w/zeros fill
                shl     bx,1                    ; Shift w/zeros fill
                shl     bx,1                    ; Shift w/zeros fill
                add     cx,bx
                pop     dx
                call    sub_20
                mov     di,cx
                mov     bl,dl
                neg     bl
                add     bl,7
                pop     es
                xor     cx,cx                   ; Zero register
                mov     cl,0Eh
                mov     si,offset data_45
                mov     dx,1

locloop_32:
                mov     ah,es:[di]
                mov     al,es:[di+1]
                xchg    cx,bx
                shl     ax,cl                   ; Shift w/zeros fill
                xchg    cx,bx
                mov     [si],ah
                inc     si
                rol     al,1                    ; Rotate
                or      dh,al
                xor     al,ah
                not     al
                and     dl,al
                add     di,2000h
                jns     loc_33                  ; Jump if not sign
                sub     di,7FA6h
loc_33:
                loop    locloop_32              ; Loop if cx > 0

                push    dx
                xor     cx,cx                   ; Zero register
                mov     cl,0Eh
                mov     dx,cx
                push    ds
                pop     es
                mov     di,offset data_47
                add     di,cx
                mov     ax,1
loc_34:
                push    di
                mov     si,offset data_45
                repe    cmpsb                   ; Rep zf=1+cx >0 Cmp [si] to es:[di]
                pop     di
                jz      loc_35                  ; Jump if zero
                inc     ax
                mov     cx,dx
                add     di,cx
                cmp     ax,0FFh
                jbe     loc_34                  ; Jump if below or =
                xor     ax,ax                   ; Zero register
loc_35:
                pop     dx
                cmp     al,0B3h
                jbe     loc_37                  ; Jump if below or =
                cmp     al,0DFh
                ja      loc_37                  ; Jump if above
                or      dl,dl                   ; Zero ?
                jnz     loc_38                  ; Jump if not zero
loc_36:
                xor     ax,ax                   ; Zero register
                jmp     short loc_38
loc_37:
                and     dh,1
                jnz     loc_36                  ; Jump if not zero
loc_38:
                mov     [bp],ax
                mov     es,data_25
                retn
                db      14 dup (0)

;ßßßß External Entry into Subroutine ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

Dot:
                mov     di,0B000h
                test    bh,1
                jz      loc_39                  ; Jump if zero
                mov     di,0B800h
loc_39:
                mov     es,di
                call    sub_20
                mov     di,cx
                mov     bx,dx
                mov     bl,data_33[bx]
                or      al,al                   ; Zero ?
                jnz     loc_40                  ; Jump if not zero
                not     bl
                and     es:[di],bl
                jmp     short loc_ret_42
                db      90h
loc_40:
                test    al,80h
                jz      loc_41                  ; Jump if zero
                xor     es:[di],bl
                jmp     short loc_ret_42
                db      90h
loc_41:
                or      es:[di],bl

loc_ret_42:
                retn

;ßßßß External Entry into Subroutine ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß

GetPoint:
                mov     di,0B000h
                test    bh,1
                jz      loc_43                  ; Jump if zero
                mov     di,0B800h
loc_43:
                mov     es,di
                call    sub_20
                mov     di,cx
                mov     bx,dx
                mov     bl,data_33[bx]
                mov     al,es:[di]
                and     al,bl
                neg     al
                sbb     al,al
                neg     al
                mov     [bp],al
                retn
                db      7 dup (0)
WriteSym          endp


;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

WriteSymAttr          proc    near
loc_44:
                or      cx,cx                   ; Zero ?
                jnz     loc_45                  ; Jump if not zero
                jmp     loc_ret_61
loc_45:
                mov     ah,bl
                push    ax
                mov     dx,0B000h
                test    bh,1
                jz      loc_46                  ; Jump if zero
                mov     dx,0B800h
loc_46:
                mov     bl,bh
                xor     bh,bh                   ; Zero register
                shl     bx,1                    ; Shift w/zeros fill
                mov     bx,es:vid_curs_pos0_[bx]
                mov     al,bh
                xor     ah,ah                   ; Zero register
                mul     data_44                 ; ax = data * al
                push    ax
                mov     es,dx
                xor     bh,bh                   ; Zero register
                mov     ax,bx
                shl     bx,1                    ; Shift w/zeros fill
                shl     bx,1                    ; Shift w/zeros fill
                shl     bx,1                    ; Shift w/zeros fill
                add     bx,ax
                mov     dx,cx
                shl     cx,1                    ; Shift w/zeros fill
                shl     cx,1                    ; Shift w/zeros fill
                shl     cx,1                    ; Shift w/zeros fill
                add     dx,cx
                mov     cx,dx
                add     cx,bx
                dec     cx
                mov     ax,bx
                shr     ax,1                    ; Shift w/zeros fill
                shr     ax,1                    ; Shift w/zeros fill
                shr     ax,1                    ; Shift w/zeros fill
                mov     dx,cx
                shr     dx,1                    ; Shift w/zeros fill
                shr     dx,1                    ; Shift w/zeros fill
                shr     dx,1                    ; Shift w/zeros fill
                sub     dx,ax
                inc     dx
                mov     data_29,dx
                xchg    bx,cx
;*              and     bx,7
                db       83h,0E3h, 07h          ;  Fixup - byte match
                neg     bx
;*              add     bx,7
                db       83h,0C3h, 07h          ;  Fixup - byte match
                mov     al,data_35[bx]
                mov     byte ptr data_41+1,al
                pop     dx
                call    sub_20
                mov     di,cx
                mov     bx,dx
                mov     data_43,bl
                mov     al,data_34[bx]
                mov     byte ptr data_41,al
                mov     bx,data_41
                or      bl,bl                   ; Zero ?
                jz      loc_47                  ; Jump if zero
                dec     data_29
loc_47:
                or      bh,bh                   ; Zero ?
                jz      loc_48                  ; Jump if zero
                dec     data_29
loc_48:
                pop     ax
                push    ax
                xor     ch,ch                   ; Zero register
                mov     cl,0Eh
                push    cx
                push    di
                test    ah,80h
                jnz     loc_53                  ; Jump if not zero
                xor     al,al                   ; Zero register

locloop_49:
                push    cx
                mov     cx,data_29
                mov     dx,di
                or      bl,bl                   ; Zero ?
                jz      loc_50                  ; Jump if zero
                and     es:[di],bl
                inc     di
loc_50:
                rep     stosb                   ; Rep when cx >0 Store al to es:[di]
                or      bh,bh                   ; Zero ?
                jz      loc_51                  ; Jump if zero
                and     es:[di],bh
loc_51:
                mov     di,dx
                add     di,data_149e
                jns     loc_52                  ; Jump if not sign
                sub     di,7FA6h
loc_52:
                pop     cx
                loop    locloop_49              ; Loop if cx > 0

loc_53:
                mov     dx,data_41
                or      dl,dl                   ; Zero ?
                jz      loc_54                  ; Jump if zero
                inc     data_29
loc_54:
                or      dh,dh                   ; Zero ?
                jz      loc_55                  ; Jump if zero
                not     dh
loc_55:
                pop     di
                pop     cx
                mov     bl,data_43
                inc     bl
                pop     ax
                xor     dl,dl                   ; Zero register
                cmp     al,0B3h
                jbe     loc_56                  ; Jump if below or =
                cmp     al,0DFh
                ja      loc_56                  ; Jump if above
                inc     dl
loc_56:
                mul     cl                      ; ax = reg * al
                mov     si,offset data_47
                add     si,ax

locloop_57:
                push    cx
                lodsb                           ; String [si] to al
                or      al,al                   ; Zero ?
                jz      loc_59                  ; Jump if zero
                mov     ah,dl
                and     ah,al
                ror     ah,1                    ; Rotate
                mov     cl,bl
                rol     ax,cl                   ; Rotate
                push    di
                mov     cx,data_29

locloop_58:
                xor     es:[di],ah
                mov     bh,al
                shr     ax,1                    ; Shift w/zeros fill
                or      ah,bh
                inc     di
                loop    locloop_58              ; Loop if cx > 0

                and     ah,dh
                xor     es:[di],ah
                pop     di
loc_59:
                add     di,2000h
                jns     loc_60                  ; Jump if not sign
                sub     di,7FA6h
loc_60:
                pop     cx
                loop    locloop_57              ; Loop if cx > 0

                push    cs
                pop     ds
                mov     es,data_25

loc_ret_61:
                retn
WriteSymAttr          endp

                db      12 dup (0)

;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

ScrollUp          proc    near
                mov     bl,bh
                push    bx
                sub     dh,ch
                inc     dh
                sub     dh,al
                mov     byte ptr data_30,dh
                xor     ah,ah                   ; Zero register
                mul     data_44                 ; ax = data * al
                mov     data_32,ax
                sub     dl,cl
                inc     dl
                xor     dh,dh                   ; Zero register
                mov     byte ptr data_30+1,dh
                mov     ax,0Eh
                mov     bx,ax
                mul     byte ptr data_30        ; ax = data * al
                mov     data_30,ax
                mov     ax,bx
                mul     ch                      ; ax = reg * al
                push    ax
                mov     ax,dx
                shl     dx,1                    ; Shift w/zeros fill
                shl     dx,1                    ; Shift w/zeros fill
                shl     dx,1                    ; Shift w/zeros fill
                add     dx,ax
                mov     data_29,dx
                xor     ch,ch                   ; Zero register
                mov     ax,cx
                shl     cx,1                    ; Shift w/zeros fill
                shl     cx,1                    ; Shift w/zeros fill
                shl     cx,1                    ; Shift w/zeros fill
                add     cx,ax
                mov     ax,cx
                pop     dx
                mov     bx,dx
                call    sub_20
                mov     di,cx
                mov     dx,bx
                add     dx,data_32
                mov     cx,ax
                call    sub_20
                mov     si,cx
                push    ax
                xor     bh,bh                   ; Zero register
                mov     bl,dl
                mov     cl,data_34[bx]
                add     ax,data_29
                dec     ax
                mov     dx,ax
                and     ax,7
                neg     ax
                add     ax,7
                mov     bx,ax
                mov     ch,data_35[bx]
                mov     data_41,cx
                shr     dx,1                    ; Shift w/zeros fill
                shr     dx,1                    ; Shift w/zeros fill
                shr     dx,1                    ; Shift w/zeros fill
                pop     ax
                shr     ax,1                    ; Shift w/zeros fill
                shr     ax,1                    ; Shift w/zeros fill
                shr     ax,1                    ; Shift w/zeros fill
                sub     dx,ax
                inc     dx
                or      cl,cl                   ; Zero ?
                jz      loc_62                  ; Jump if zero
                dec     dx
loc_62:
                or      ch,ch                   ; Zero ?
                jz      loc_63                  ; Jump if zero
                dec     dx
loc_63:
                mov     data_29,dx
                mov     bx,data_41
                mov     cx,0B800h
                test    byte ptr es:video_page_,1
                jnz     loc_64                  ; Jump if not zero
                mov     cx,0B000h
loc_64:
                mov     es,cx
                mov     ds,cx
                xor     cx,cx                   ; Zero register
                add     cx,cs:data_32
                jnz     loc_65                  ; Jump if not zero
                mov     cx,cs:data_30
                jmp     short loc_71
                db      90h
loc_65:
                mov     cx,cs:data_30

locloop_66:
                push    cx
                push    si
                push    di
                or      bl,bl                   ; Zero ?
                jz      loc_67                  ; Jump if zero
                mov     ah,es:[di]
                and     ah,bl
                lodsb                           ; String [si] to al
                not     bl
                and     al,bl
                not     bl
                or      al,ah
                stosb                           ; Store al to es:[di]
loc_67:
                mov     cx,cs:data_29
                rep     movsb                   ; Rep when cx >0 Mov [si] to es:[di]
                or      bh,bh                   ; Zero ?
                jz      loc_68                  ; Jump if zero
                mov     ah,es:[di]
                and     ah,bh
                lodsb                           ; String [si] to al
                not     bh
                and     al,bh
                not     bh
                or      al,ah
                stosb                           ; Store al to es:[di]
loc_68:
                pop     di
                pop     si
                add     si,2000h
                jns     loc_69                  ; Jump if not sign
                sub     si,7FA6h
loc_69:
                add     di,data_147e
                jns     loc_70                  ; Jump if not sign
                sub     di,7FA6h
loc_70:
                pop     cx
                loop    locloop_66              ; Loop if cx > 0

                mov     cx,cs:data_32
loc_71:
                pop     si

locloop_72:
                push    cx
                push    di
                or      bl,bl                   ; Zero ?
                jz      loc_73                  ; Jump if zero
                mov     ah,es:[di]
                and     ah,bl
                mov     dx,si
                not     bl
                and     dl,bl
                not     bl
                or      ah,dl
                xchg    ah,al
                stosb                           ; Store al to es:[di]
loc_73:
                mov     cx,cs:data_29
                mov     ax,si
                rep     stosb                   ; Rep when cx >0 Store al to es:[di]
                or      bh,bh                   ; Zero ?
                jz      loc_74                  ; Jump if zero
                mov     ah,es:[di]
                and     ah,bh
                mov     dx,si
                not     bh
                and     dl,bh
                not     bh
                or      ah,dl
                xchg    ah,al
                stosb                           ; Store al to es:[di]
loc_74:
                pop     di
                add     di,2000h
                jns     loc_75                  ; Jump if not sign
                sub     di,7FA6h
loc_75:
                pop     cx
                loop    locloop_72              ; Loop if cx > 0

                push    cs
                pop     ds
                mov     es,data_25
                retn
ScrollUp          endp


;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

ScrollDn          proc    near
                mov     bl,bh
                push    bx
                sub     dh,ch
                inc     dh
                add     ch,dh
                dec     ch
                sub     dh,al
                mov     byte ptr data_30,dh
                xor     ah,ah                   ; Zero register
                mul     data_44                 ; ax = data * al
                mov     data_32,ax
                sub     dl,cl
                inc     dl
                xor     dh,dh                   ; Zero register
                mov     byte ptr data_30+1,dh
                mov     ax,0Eh
                mov     bx,ax
                mul     byte ptr data_30        ; ax = data * al
                mov     data_30,ax
                mov     ax,bx
                mul     ch                      ; ax = reg * al
                add     ax,0Dh
                push    ax
                mov     ax,dx
                shl     dx,1                    ; Shift w/zeros fill
                shl     dx,1                    ; Shift w/zeros fill
                shl     dx,1                    ; Shift w/zeros fill
                add     dx,ax
                mov     data_29,dx
                xor     ch,ch                   ; Zero register
                mov     ax,cx
                shl     cx,1                    ; Shift w/zeros fill
                shl     cx,1                    ; Shift w/zeros fill
                shl     cx,1                    ; Shift w/zeros fill
                add     cx,ax
                mov     ax,cx
                pop     dx
                mov     bx,dx
                call    sub_20
                mov     di,cx
                mov     dx,bx
                sub     dx,data_32
                mov     cx,ax
                call    sub_20
                mov     si,cx
                push    ax
                xor     bh,bh                   ; Zero register
                mov     bl,dl
                mov     cl,data_34[bx]
                add     ax,data_29
                dec     ax
                mov     dx,ax
                and     ax,7
                neg     ax
                add     ax,7
                mov     bx,ax
                mov     ch,data_35[bx]
                mov     data_41,cx
                shr     dx,1                    ; Shift w/zeros fill
                shr     dx,1                    ; Shift w/zeros fill
                shr     dx,1                    ; Shift w/zeros fill
                pop     ax
                shr     ax,1                    ; Shift w/zeros fill
                shr     ax,1                    ; Shift w/zeros fill
                shr     ax,1                    ; Shift w/zeros fill
                sub     dx,ax
                inc     dx
                or      cl,cl                   ; Zero ?
                jz      loc_76                  ; Jump if zero
                dec     dx
loc_76:
                or      ch,ch                   ; Zero ?
                jz      loc_77                  ; Jump if zero
                dec     dx
loc_77:
                mov     data_29,dx
                mov     bx,data_41
                mov     cx,0B800h
                test    byte ptr es:video_page_,1
                jnz     loc_78                  ; Jump if not zero
                mov     cx,0B000h
loc_78:
                mov     es,cx
                mov     ds,cx
                xor     cx,cx                   ; Zero register
                add     cx,cs:data_32
                jnz     loc_79                  ; Jump if not zero
                mov     cx,cs:data_30
                jmp     short loc_85
                db      90h
loc_79:
                mov     cx,cs:data_30

locloop_80:
                push    cx
                push    si
                push    di
                or      bl,bl                   ; Zero ?
                jz      loc_81                  ; Jump if zero
                mov     ah,es:[di]
                and     ah,bl
                lodsb                           ; String [si] to al
                not     bl
                and     al,bl
                not     bl
                or      al,ah
                stosb                           ; Store al to es:[di]
loc_81:
                mov     cx,cs:data_29
                rep     movsb                   ; Rep when cx >0 Mov [si] to es:[di]
                or      bh,bh                   ; Zero ?
                jz      loc_82                  ; Jump if zero
                mov     ah,es:[di]
                and     ah,bh
                lodsb                           ; String [si] to al
                not     bh
                and     al,bh
                not     bh
                or      al,ah
                stosb                           ; Store al to es:[di]
loc_82:
                pop     di
                sub     di,2000h
                jns     loc_83                  ; Jump if not sign
                add     di,data_148e
loc_83:
                pop     si
                sub     si,2000h
                jns     loc_84                  ; Jump if not sign
                add     si,7FA6h
loc_84:
                pop     cx
                loop    locloop_80              ; Loop if cx > 0

                mov     cx,cs:data_32
loc_85:
                pop     si

locloop_86:
                push    cx
                push    di
                or      bl,bl                   ; Zero ?
                jz      loc_87                  ; Jump if zero
                mov     ah,es:[di]
                and     ah,bl
                mov     dx,si
                not     bl
                and     dl,bl
                not     bl
                or      ah,dl
                xchg    ah,al
                stosb                           ; Store al to es:[di]
loc_87:
                mov     cx,cs:data_29
                mov     ax,si
                rep     stosb                   ; Rep when cx >0 Store al to es:[di]
                or      bh,bh                   ; Zero ?
                jz      loc_88                  ; Jump if zero
                mov     ah,es:[di]
                and     ah,bh
                mov     dx,si
                not     bh
                and     dl,bh
                not     bh
                or      ah,dl
                xchg    ah,al
                stosb                           ; Store al to es:[di]
loc_88:
                pop     di
                sub     di,2000h
                jns     loc_89                  ; Jump if not sign
                add     di,7FA6h
loc_89:
                pop     cx
                loop    locloop_86              ; Loop if cx > 0

                push    cs
                pop     ds
                mov     es,data_25
                retn
ScrollDn          endp

                db      7 dup (0)

;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

tty          proc    near
                mov     cl,bl
                xor     bx,bx                   ; Zero register
                mov     bl,es:video_page_
                shl     bx,1                    ; Shift w/zeros fill
                mov     dx,es:vid_curs_pos0_[bx]
                cmp     al,7
                je      loc_91                  ; Jump if equal
                cmp     al,8
                je      loc_92                  ; Jump if equal
                cmp     al,0Ah
                je      loc_94                  ; Jump if equal
                cmp     al,0Dh
                je      loc_93                  ; Jump if equal
                mov     bl,cl
                mov     cx,1
                mov     bh,es:video_page_
                call    WriteSymAttr
                xor     bx,bx                   ; Zero register
                mov     bl,es:video_page_
                shl     bx,1                    ; Shift w/zeros fill
                mov     dx,es:vid_curs_pos0_[bx]
                inc     dl
                cmp     dl,50h                  ; 'P'
                jge     loc_95                  ; Jump if > or =
loc_90:
                mov     bh,es:data_40
                call    SetCurPos
                jmp     short loc_ret_97
                db      90h
loc_91:
                mov     ah,0Eh
                pushf                           ; Push flags
                call    OldVec10h
                jmp     short loc_ret_97
                db      90h
loc_92:
                cmp     dl,0
                je      loc_ret_97              ; Jump if equal
                dec     dl
                jmp     short loc_90
loc_93:
                xor     dl,dl                   ; Zero register
                jmp     short loc_90
loc_94:
                mov     cx,19h
                dec     cl
                cmp     dh,cl
                je      loc_96                  ; Jump if equal
                inc     dh
                jmp     short loc_90
loc_95:
                xor     dl,dl                   ; Zero register
                mov     cx,19h
                dec     cl
                cmp     dh,cl
                jge     loc_96                  ; Jump if > or =
                inc     dh
                jmp     short loc_90
loc_96:
                mov     bh,es:data_40
                call    SetCurPos
                mov     ax,1
                xor     cx,cx                   ; Zero register
                xor     bh,bh                   ; Zero register
                mov     dx,184Fh
                call    ScrollUp

loc_ret_97:
                retn
tty          endp

                db      10 dup (0)

;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

sub_19          proc    near
                mov     dx,3BAh

locloop_98:
                in      al,dx                   ; port 3BAh, MDA/EGA vid status
                or      al,al                   ; Zero ?
                js      loc_99                  ; Jump if sign=1
                jmp     short locloop_98
loc_99:
                in      al,dx                   ; port 3BAh, MDA/EGA vid status
                or      al,al                   ; Zero ?
                jns     loc_100                 ; Jump if not sign
                jmp     short loc_99
loc_100:
                loop    locloop_98              ; Loop if cx > 0

                retn
sub_19          endp


;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              SUBROUTINE
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

sub_20          proc    near
                push    si
                mov     si,dx
                shl     si,1                    ; Shift w/zeros fill
                mov     dx,data_36[si]
                push    cx
                shr     cx,1                    ; Shift w/zeros fill
                shr     cx,1                    ; Shift w/zeros fill
                shr     cx,1                    ; Shift w/zeros fill
                add     cx,dx
                pop     dx
;*              and     dx,7
                db       83h,0E2h, 07h          ;  Fixup - byte match
                neg     dx
;*              add     dx,7
                db       83h,0C2h, 07h          ;  Fixup - byte match
                pop     si
                retn
sub_20          endp

                db      15 dup (0)

;ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
;                              MAIN
;ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ

StayResident:   ; à®¢¥àª , ¬®¦¥â ¬ë ã¦¥ à¥§¨¤¥­â­ë?
                mov     dl,0FFh
                mov     ah,0EFh
                int     10h
                cmp     dl,0FFh
                mov     dx,offset s_already
                jne     AlreadyStayResident
                call    TestHerc   ; ¯à®¢¥àª  ­  ­ «¨ç¨¥ ƒ¥àªã«¥á 
                cmp     dl,0FFh
                jne     HercPresent
                mov     dx,offset s_not_herc
AlreadyStayResident:
                mov     ah,9
                int     21h     ; DOS Services display char string at ds:dx
                xor     ax,ax
                int     20h     ; DOS program terminate
HercPresent:
                ; ‘ª ­¨àã¥¬ ª®¬ ­¤­ãî áâà®ªã ­  ä« £¨
                cld
                xor     cx,cx
                mov     cl,80h

                jcxz    loc_105
                mov     di,data_3e      ; = 81H -  ¤à¥á ª®¬ áâà®ª¨ ¢ PSP
loc_104:
                mov     al,2Fh          ; '/'
                repne   scasb           ; Rep zf=0+cx >0 Scan es:[di] for al
                jcxz    loc_105
                mov     al,[di]         ; ä« £
                and     al,0DFh         ; uppercase
                cmp     al,48h          ; 'H'
                jne     loc_104
                mov     FlagH,1         ; H ©¤¥­ ä« £ /H
loc_105:        ; save old vector of int 10h
                push    es
                xor     ax,ax
                mov     es,ax                   ; ES:=0
                mov     bx,es:Vec10h            ; bx := 0:40
                mov     word ptr OldVec10h,bx
                mov     bx,es:Vec10h+2
                mov     word ptr OldVec10h+2,bx
                ; set new vector of unt 10h
                cli
                mov     word ptr es:Vec10h,offset Trap10h
                mov     es:Vec10h+2,cs
                sti
                pop     es
                mov     ah,9
                mov     dx,offset s_OK
                int     21h     ;
                ; calculate resident memory
                mov     dx,offset StayResident  ; 1A20h
                mov     cl,4
                shr     dx,cl                   ; Shift w/zeros fill
                inc     dx
                mov     ax,3100h
                int     21h     ; TSR al=return code,dx=paragraphs
s_OK       db 'Hercules Resident Video Support Routines. Version 1.10',0Dh,0Ah
           db '$'
s_already  db 'Hercules Video Support Routines are already installed.',0Dh,0Ah
           db '$'
s_not_herc db 'Hercules Video Card not present.',0Dh,0Ah
           db 'Hercules Video Support Routines not installed.', 0Dh, 0Ah, '$'
seg_a      ends
           end     start
