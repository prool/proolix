#include "mse.h"
#include <conf.c>
#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <dos.h>
#include <dir.h>
#include <fcntl.h>
#include <alloc.h>
#include <dirent.h>
#include <proto.h>
/****************************************************************************/
void dos_fn_man (char fn);
/****************************************************************************/
char str [STRLEN];
/****************************************************************************/
void dosname2pix(char *pix_name,char far *dos_name,int pix_len)
{

#if 0
if (msdos_verbose)
  {
  #ifndef _HERCULES_
  textattr(EMULATOR_COLOR);
  #endif
  printf(" `%Fs'->",dos_name);
  }
#endif

/* MSDOS reserved names */

if (strcmp(dos_name,"lpt1")) {strncpy(pix_name,"/dev/lpt",pix_len); return; }
if (strcmp(dos_name,"lpt2")) {strncpy(pix_name,"/dev/lpt",pix_len); return; }

if (*(dos_name+1)==':') dos_name+=2;
strncpy(pix_name,dos_name,pix_len);
strlwr(pix_name);
while (*pix_name)
  {
  if (*pix_name=='\\') *pix_name='/';
  pix_name++;
  }

#if 0
if (msdos_verbose)
  {
  printf(" `%s'->",pix_name);
  #ifndef _HERCULES_
  textattr(DOS_COLOR);
  #endif
  }
#endif

}
/****************************************************************************/
void puts_dos (const char far *str)
{
char c;
while((c=*str++)!='$')
  putch(c);
}
/****************************************************************************/
void interrupt Int27(void) /* TSR */
{
TSR=1;
Int20();
}
/****************************************************************************/
void interrupt Int20(void)
{
if (aPSP) if (TSR==0) free((void far *)aPSP);
longjmp(return_point, 0);
}
/****************************************************************************/
void interrupt Int23(void)
{
printf("\bBREAK\n");
Int20();
}
/****************************************************************************/
void interrupt Int24(void)
{
printf("\bFatal error\n");
Int20();
}
/****************************************************************************/
void interrupt Int29(void)
{
printf("Int29 called\n");
}
/****************************************************************************/
void interrupt Int21(void)
{
char huge * Pointer;
char far * far_ptr, far * far_ptr2;
unsigned long ll;
DIR far *d; struct dirent far * Rec;
char *cc;
unsigned int iax, ibx, icx, idx, ids, ies, isi, ifl, i, dos_fn;
void interrupt (*Vec) (void);
struct DTAfind far * DTA2;

/*
        flags                   ; [BP+22]

      From [HelpPC]:

      ³11³10³F³E³D³C³B³A³9³8³7³6³5³4³3³2³1³0³
        ³  ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ÀÄÄÄ  CF Carry Flag
        ³  ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ÀÄÄÄ  1
        ³  ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ÀÄÄÄ  PF Parity Flag
        ³  ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ÀÄÄÄ  0
        ³  ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ÀÄÄÄ  AF Auxiliary Flag
        ³  ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ÀÄÄÄ  0
        ³  ³ ³ ³ ³ ³ ³ ³ ³ ³ ³ ÀÄÄÄ  ZF Zero Flag
        ³  ³ ³ ³ ³ ³ ³ ³ ³ ³ ÀÄÄÄ  SF Sign Flag
        ³  ³ ³ ³ ³ ³ ³ ³ ³ ÀÄÄÄ  TF Trap Flag  (Single Step)
        ³  ³ ³ ³ ³ ³ ³ ³ ÀÄÄÄ  IF Interrupt Flag
        ³  ³ ³ ³ ³ ³ ³ ÀÄÄÄ  DF Direction Flag
        ³  ³ ³ ³ ³ ³ ÀÄÄÄ  OF Overflow flag
        ³  ³ ³ ³ ÀÄÁÄÄÄ  IOPL I/O Privilege Level  (286+ only)
        ³  ³ ³ ÀÄÄÄÄÄ  NT Nested Task Flag  (286+ only)
        ³  ³ ÀÄÄÄÄÄ  0
        ³  ÀÄÄÄÄÄ  RF Resume Flag (386+ only)
        ÀÄÄÄÄÄÄ  VM  Virtual Mode Flag (386+ only)

        return addr             ; offset == [BP+20]
                                ; segment == [BP+18]
        push    ax              ; AX == [BP+16]
        push    bx              ; BX == [BP+14]
        push    cx              ; CX == [BP+12]
        push    dx              ; DX == [BP+10]
        push    es              ; ES == [BP+8]
        push    ds              ; DS == [BP+6]
        push    si              ; SI == [BP+4]
        push    di              ; DI == [BP+2]
        push    bp              ; BP == [BP]
        mov     bp,DGROUP
        mov     DS,bp
        mov     bp,sp
        sub     sp,6
        */

asm     mov     iax,ax;
asm     mov     ibx,bx;
asm     mov     icx,cx;
asm     mov     idx,dx;
asm     mov     ies,es;
asm     mov     isi,si;

asm     mov     ax,word ptr [BP+22];
asm     mov     ifl,ax;

asm     mov     ax,word ptr [BP+6];
asm     mov     ids,ax;

dos_fn=(iax>>8)&0xFF;

if (allow_dos_functions[dos_fn])
  {
  if (msdos_verbose) if (verbose_dos_functions[dos_fn])
    {
    #ifndef _HERCULES_
    textattr(EMULATOR_COLOR);
    #endif
    dos_fn_man(dos_fn);
    #ifndef _HERCULES_
    textattr(DOS_COLOR);
    #endif
    }
  switch (dos_fn)
    {
    case 1:   i=getch(); putch(i);
              asm       mov       ax,i;
              asm       mov       byte ptr [BP+16],al;
              break;
    case 2:   putch((char)idx); break;
    case 6:   if (((unsigned char)idx)==0xFFU)
                {
                if (kbhit())
                  {
                  /* ZF = 0 */
                  ifl=ifl & ~0x40;
                  asm       mov     ax,ifl;
                  asm       mov     word ptr [BP+22],ax;
                  i=getch();
                  asm       mov       ax,i;
                  asm       mov       byte ptr [BP+16],al;
                  }
                else
                  {
                  /* ZF = 1 */
                  ifl=ifl | 0x40;
                  asm       mov     ax,ifl;
                  asm       mov     word ptr [BP+22],ax;
                  }
                }
              else
                putch((char)idx);
              break;
    case 7:
    case 8:   i=getch();
              asm       mov       ax,i;
              asm       mov       byte ptr [BP+16],al;
              break;
    case 9:   puts_dos(MK_FP(ids,idx)); break;
    case 0xB: if (kbhit()) i=0xFF; else i=0;
              asm       mov       ax,i;
              asm       mov       byte ptr [BP+16],al;
              break;
    case 0xE: /* */
              asm        mov     byte ptr [BP+16],3;
              break;
    case 0x19:asm     mov     word ptr [BP+16],2;
              break;
    case 0x1A:DTAaddr=MK_FP(ids,idx);
              break;
    case 0x25:if (allow_change_int[(char)iax])
                {
                if (msdos_verbose)
                  printf(" int %02X vec ",(char)iax);
                setvect((char)iax,(void interrupt(*)(void))MK_FP(ids,idx));
                }
              else
                if (msdos_verbose)
                  printf("\nDisallow change int %02X vec",(char)iax);
                else ;
              break;
    case 0x29:/* Parse a Filename for FCB */
              printf("Parse=%Fs\n",MK_FP(ids,isi));
              break;
    case 0x2A:/* get date */
                {struct date datep;
                getdate(&datep);
                i=datep.da_year;
                asm     mov     ax,i;
                asm     mov     word ptr [BP+12],ax;
                i=datep.da_day;
                asm     mov     ax,i;
                asm     mov     byte ptr [BP+10],al;
                i=datep.da_mon;
                asm     mov     ax,i;
                asm     mov     byte ptr [BP+11],al;
                }
              break;
    case 0x2B:/* set date */
              /* nothing does */
              asm       mov     byte ptr [BP+16],0;
              break;
    case 0x2C: /* get time */
                {struct time timep;
                gettime(&timep);
                i=timep.ti_min;
                asm     mov     ax,i;
                asm     mov     byte ptr [BP+12],al;
                i=timep.ti_hour;
                asm     mov     ax,i;
                asm     mov     byte ptr [BP+13],al;
                i=timep.ti_sec;
                asm     mov     ax,i;
                asm     mov     byte ptr [BP+11],al;
                i=timep.ti_hund;
                asm     mov     ax,i;
                asm     mov     byte ptr [BP+10],al;
                }
              break;
    case 0x2F:i=FP_OFF(DTAaddr);
              asm       mov     ax,i;
              asm       mov     word ptr [BP+14],ax;
              i=FP_SEG(DTAaddr);
              asm       mov     ax,i;
              asm       mov     word ptr [BP+8],ax;
              break;
    case 0x30:i=((osminor*10)<<8)+osmajor;
              asm       mov     ax,i;
              asm       mov     word ptr [BP+16],ax;
              asm       mov     word ptr [BP+14],0;
              break;
    case 0x31:/* TSR */
              TSR=1;
              printf("TSR\n");
              Int20();
    case 0x33:/* Get/set Ctrl-Break check & get boot drive */
              switch ((char)iax)
                {
                case 1 : /* set Ctrl-Break checking flag */
                         osbreak=(char)idx;
                case 0 : /* get Ctrl-Break checking flag */
                         asm    mov     al,osbreak;
                         asm    mov     byte ptr [BP+10],al;
                         break;
                case 2 : /* set extended Ctrl-Break checking */
                         break;
                case 5 : /* get boot drive (DOS 4.x) */
                        asm     mov     byte ptr [BP+10],3;
                default:;
                }
              break;
    case 0x35:/* get vector */
              if (msdos_verbose)
                printf(" int %02X vec ",(char)iax);
              Vec=getvect((char)iax);
              i=FP_SEG(Vec);
              asm   mov     ax,i;
              asm   mov     [BP+8],ax;
              i=FP_OFF(Vec);
              asm   mov     ax,i;
              asm   mov     [BP+14],ax;
              break;
    case 0x37:switch((char)iax)
                {
                case 2 :
                case 0 : asm    mov     byte ptr [BP+10],'-';
                         break;
                case 1 :
                case 3 :
                default:
                         printf("DOS Fn 37, subfn=%02X\n",(char)iax);
                }
              break;
    case 0x38:
              #if 0
              switch((char)iax)
                {
                case 0: /* get contry info */
                        asm     mov     word ptr [BP+16],0; /* err code=0 */
                        asm     mov     word ptr [BP+14],0; /* country code=0 */
                        /* ds:dx - contry info */
                        i=FP_OFF(&Ukraine);
                        asm     mov     ax,i;
                        asm     mov     word ptr [BP+10],ax; /* dx */
                        asm     push    DS;
                        asm     pop     ax;
                        asm     mov     word ptr [BP+6],ax; /* DS */
                        goto l_no_error;
                default:printf("Get/set country information. Fn=%i\n",
                               (char)iax);
                }
              #endif
              break;
    case 0x3B:if (msdos_verbose) printf("chdir %Fs",MK_FP(ids,idx));
              dosname2pix(str,MK_FP(ids,idx),STRLEN);
              if (chdir(str)==-1) goto l_error;
              goto l_no_error;
    case 0x3C:/* Create */
              dosname2pix(str,MK_FP(ids,idx),100);
              if (msdos_verbose)
                {
                #ifndef _HERCULES_
                textattr(EMULATOR_COLOR);
                #endif
                printf("`%s'\n",str);
                #ifndef _HERCULES_
                textattr(DOS_COLOR);
                #endif
                }
              if ((i=open(str,O_WRONLY|O_CREAT))==-1U)
                {/* error */
                l_error:
                asm       mov     word ptr [BP+16],1; /* errno=1 */
                /* CF = 1 */
                ifl=ifl | 1;
                asm       mov     ax,ifl;
                asm       mov     word ptr [BP+22],ax;
                }
              else
                {/* no error */
                asm       mov     ax,i;
                asm       mov     [BP+16],ax;
                l_no_error:
                /* CF = 0 */
                ifl=ifl & 0xFFFE;
                asm       mov     ax,ifl;
                asm       mov     word ptr [BP+22],ax;
                }
              break;
    case 0x3D:/* Open */
              if (msdos_verbose) printf(" open. mode=%04X ",(char)iax);
              dosname2pix(str,MK_FP(ids,idx),100);
              switch ((char)iax)
                {
                case 1: i=O_WRONLY; break;
                case 2: i=O_RDWR; break;
                case 0: i=O_RDONLY; break;
                default:i=O_RDONLY;
                }
              if (msdos_verbose)
                {
                #ifndef _HERCULES_
                textattr(EMULATOR_COLOR);
                #endif
                printf("`%s'\n",str);
                #ifndef _HERCULES_
                textattr(DOS_COLOR);
                #endif
                }
              if ((i=open(str,i))==-1U)
                goto l_error;
              else
                {/* no error */
                asm       mov     ax,i;
                asm       mov     [BP+16],ax;
                goto l_no_error;
                }
    case 0x3E:/* close */
              switch(ibx)
                {
                case 0 : /* stdin */
                case 1 : /* stdout */
                case 2 : /* stderr */
                case 3 : /* stdaux */
                case 4 : /* stdprn */
                         printf("close handle 0-4\n");
                default: ;
                }
              if (close(ibx)) goto l_error;
              else goto l_no_error;
    case 0x3F:/* read */
              if (msdos_verbose) printf("read h=%i len=%i\n",ibx,icx);
              Pointer=MK_FP(ids,idx);
              switch(ibx)
                {
                case 0 : /* stdin */
                         for (i=0;i<icx;i++)
                           {char c;
                           c=getche();
                           *Pointer++=c;
                           if (c=='\r') break;
                           }
                         asm       mov     ax,i;
                         asm       mov     [BP+16],ax;
                         goto l_no_error;
                case 1 : /* stdout */
                case 2 : /* stderr */
                case 4 : /* stdprn */
                         goto l_error;
                case 3 : /* stdaux */
                         printf("read from stdaux !!!\n");
                default: if ((i=(int)read(ibx,(char far *)Pointer,icx))==-1U)
                           goto l_error;
                         else
                           {/* no error */
                           asm       mov     ax,i;
                           asm       mov     [BP+16],ax;
                           goto l_no_error;
                           }
                }
    case 0x40:/* Write */
              if (msdos_verbose) printf("write h=%i len=%i\n",ibx,icx);
              Pointer=MK_FP(ids,idx);
              switch(ibx)
                {
                case 0 : /* stdin */
                         goto l_error;
                case 1 : /* stdout */
                case 2 : /* stderr */
                         for (i=0;i<icx;i++)
                           {
                           putch(*Pointer);
                           }
                         asm       mov     ax,icx;
                         asm       mov     [BP+16],ax;
                         goto l_no_error;
                case 4 : /* stdprn */
                case 3 : /* stdaux */
                         printf("write to stdprn or stdaux !!!\n");
                default: if ((i=(int)write(ibx,(char far *)Pointer,icx))==-1U)
                           goto l_error;
                         else
                           {/* no error */
                           asm       mov     ax,i;
                           asm       mov     [BP+16],ax;
                           goto l_no_error;
                           }
                }
    case 0x42:/* seek */
              switch(ibx)
                {
                case 0 : /* stdin */
                case 1 : /* stdout */
                case 2 : /* stderr */
                case 3 : /* stdaux */
                case 4 : /* stdprn */
                         goto l_error;
                default: ;
                }
              if((ll=lseek(ibx,(((unsigned long)icx)<<(unsigned)16)|idx,
              (char)iax))==-1L)
                goto l_error;
              else
                {
                i=(unsigned int)ll;
                asm     mov     ax,i;
                asm     mov     word ptr [BP+16],ax;
                i=(unsigned int)(ll>>16);
                asm     mov     ax,i;
                asm     mov     word ptr [BP+10],ax;
                goto l_no_error;
                }
    case 0x44:if (msdos_verbose)
                printf("IOCTL Fn=%02X handle=%04X\n",(char)iax,ibx);
              switch((char)iax)
                {
                case   0: switch (ibx)
                            {
                            case 0 :
                            case 1 :
                            case 2 :asm   mov     word ptr [BP+10],0x80d3;
                                    break;
                            case 3 :asm   mov     word ptr [BP+10],0x80c0;
                                    break;
                            case 4 :asm   mov     word ptr [BP+10],0xa0c0;
                                    break;
                            default:
                                    asm   mov     word ptr [BP+10],3;
                            }
                          break;
                case   1:
                case   2:
                case   3:
                case   4:
                case   5:
                case   6:
                case   7:
                case   8:
                case   9:
                case 0xa:
                case 0xb:
                case 0xc:
                case 0xd:
                case 0xe:
                case 0xf:
                          #ifndef _HERCULES_
                          textattr(EMULATOR_COLOR);
                          #endif
                          printf("IOCTL Fn=%02X\n",(char)iax);
                          #ifndef _HERCULES_
                          textattr(DOS_COLOR);
                          #endif
                          break;
                default :
                          #ifndef _HERCULES_
                          textattr(EMULATOR_COLOR);
                          #endif
                          printf("IOCTL invalid Fn=%02X\n",(char)iax);
                          #ifndef _HERCULES_
                          textattr(DOS_COLOR);
                          #endif
                }
              goto l_no_error;
    case 0x47:if (getcwd(str,64))
                goto l_error;
              else
                {/* no error */
                strupr(str);
                for (i=1;i<64;i++)if (str[i]=='/') str[i]='\\';
                #if 0
                if (str[0]=='\\')
                  strncpy(MK_FP(ids,isi),str+1,64);
                else
                  strncpy(MK_FP(ids,isi),str,64);
                #endif
                asm       mov     ax,i;
                asm       mov     [BP+16],ax;
                goto l_no_error;
                }
    case 0x48:far_ptr=malloc(((long)ibx)<<4);
              i=FP_SEG(far_ptr);
              if (far_ptr==NULL)
                goto l_error;
              else
                {/* no error */
                asm       mov     ax,i;
                asm       mov     [BP+16],ax;
                goto l_no_error;
                }
    case 0x49:if (msdos_verbose)
                printf("free %04X\n",ies);
              break;
    case 0x4A:/* sbrk */
              if(realloc(MK_FP(ies,0),(long)ibx<<4)==NULL)
                goto l_error;
              else
                goto l_no_error;
    case 0x4E:/* Find First */
              if (msdos_verbose) printf(" find first `%Fs'",MK_FP(ids,idx));
              dosname2pix(str,MK_FP(ids,idx),STRLEN);
              cc=(char *)strstr(str,"*.*");
              if (cc!=NULL) *cc=0;
              i=(int)strlen(str);
              if (i!=0) i--;
              if (i!=0) if (str[i]=='/') str[i]=0;
              if (str[0]==0)
                {
                if (msdos_verbose) printf("L1");
                memcpy(str,".",2);
                }
              /* if (msdos_verbose) printf("=>`%s' ",str); */
              if ((d=opendir(str))==NULL) goto l_error;
              ids=FP_SEG(DTAaddr);
              idx=FP_OFF(DTAaddr);
    case 0x4F:/* Find Next */
              DTA2=MK_FP(ids,idx);
              while (1)
                {
                if ((Rec=readdir(d))==NULL) {closedir(d); goto l_error; }
                if ((Rec->filename[0]!=0)&&(Rec->filename[0]!=0xE5)) break;
                }
              DirToPath(Rec, str);
              strupr(str);
              strncpy(DTA2->filename,str,13);
              /* if (msdos_verbose) printf(" str=%s->%Fs ",str,DTA2->filename); */
              DTA2->attr=Rec->attrib.U;
              goto l_no_error;
    case 0:   iax=0;
    case 0x4C:TSR=0;
              Int20();
    case 0x58:if ((char)iax)
                {
                if (!ibx)
                  {
                  printf("\nSet strategy - ");
                  switch (ibx)
                    {
                    case 0 : printf("1st "); break;
                    case 1 : printf("best "); break;
                    case 2 : printf("last "); break;
                    default: printf("? "); break;
                    }
                  }
                }
              else
                {
                if (msdos_verbose) printf("\nGet strategy");
                asm     mov     word ptr [BP+16],0;
                }
              /* getch(); */
              break;
    case 0x62:/* get PSP addr */
              i=FP_SEG(aPSP);
              asm       mov       ax,i;
              asm       mov     word ptr [BP+14],ax;
              break;
    default  :if (1 /*msdos_verbose*/)
                {
                #ifndef _HERCULES_
                textattr(EMULATOR_COLOR);
                #endif
                printf("\nNot realized MSDOS Fn %02X ",dos_fn);
                dos_fn_man(dos_fn);
                #ifndef _HERCULES_
                textattr(DOS_COLOR);
                #endif
                }
    }
  }
else
  {
  #ifndef _HERCULES_
  textattr(EMULATOR_COLOR);
  #endif
  printf("\nDisallow fn %02X ",dos_fn);
  dos_fn_man(dos_fn);
  #ifndef _HERCULES_
  textattr(DOS_COLOR);
  #endif
  }
if (msdos_pause) getch();
}