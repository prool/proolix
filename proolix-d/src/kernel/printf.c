#include <stdarg.h>

#include <prool.h>

#include <conf.c>
#include "kernel.h"
/****************************************************************************/
/* #define DEBUG */
/****************************************************************************/
int printf (const char far *format, ...)
{
va_list v; int counter;
char new_printf_buf[MAX_LEN_STR];

/* Called functions: vsprintf(), Puts0() */

/* Использование в качестве буфера массива printf_buf, выделяемого по
malloc (см. в kernel.c) вызывает глюки (зависания).
А использование в качестве буфера
локального массива new_printf_buf глюков не вызывает. Все работает. */

va_start(v,0);
counter=vsprintf(new_printf_buf,format,v);
if(Puts0(new_printf_buf)==EOF) return EOF;
return counter;
}
/****************************************************************************/
int vsprintf (char far *str, const char far *format, va_list v)
{
int i, counter=0, width, prec, Modifier, Type,
  FlagMinus, FlagPlus, FlagBlank, FlagDiez, FlagZero;
char c, *cc, far *fcc;
unsigned int segment;
int len; char far *ss; long L;
/* char Clipboard [MAX_LEN_STR]; */
char new_printf_buf[MAX_LEN_STR]; /* см. примечание в начале ф-ции printf */

while(*format)
  {
  if (*format!='%')
    {
    *str++=*format;
    counter++;
    }
  else
    {char s [] = "printf: Flag '%c' is not realized now :( ";
    /* встречен символ '%' */
    /* % [FLAG] [WIDTH] [.PREC] [MODIFIER] TYPE */
    /*
                                TYPE:
       d signed dec int
       i "
       o unsigned oct int
       u unsigned dec int
       x unsigned hex int lowercase
       X "                uppercase
       f float
       e float with Exponent
       g f or e
       E e
       G g
       c 1 char
       s string
       % %
       p pointer
       n char counter (?)

                                FLAG:
       none right, 0 or blank
       - left
       + sign + or -
       blank sign - only
       # oct - 0 hex - 0x 0X
                                WIDTH:
       n
       0n 0 fill
       * next arg - width
                                PREC:
       none
       .0
       .n
       * - next arg
                                MODIFIER:
       F far
       N near
       h short int
       l long int
       L long double
     */
    /* обрабатываем флаги */
    /* % [FLAG] [WIDTH] [.PREC] [MODIFIER] TYPE */
    FlagMinus=0; FlagPlus=0; FlagBlank=0; FlagDiez=0; FlagZero=0;
    while(strchr("-+ #0",c=*++format)!=NULL)
      switch (c)
      {
      /* данная идиотская запись вида "var=1; if (var)" используется для того,
      чтобы не было сообщения компилятора "variable not used". После того,
      как обработка флагов будет реализована, идиотские операторы будут,
      естественно, удалены */
        case '-': FlagMinus=1; break;
        case '+': FlagPlus =1; if (FlagPlus ) printf(s,c); break;
        case ' ': FlagBlank=1; if (FlagBlank) printf(s,c); break;
        case '#': FlagDiez =1; if (FlagDiez ) printf(s,c); break;
        case '0': FlagZero =1;
      }
    if (c==0) break;
    /* Обрабатываем ширину */
    /* % [FLAG] [WIDTH] [.PREC] [MODIFIER] TYPE */
    if (isdigit(*format))
      {width=atoi(format);while(isdigit(c=*++format));
      }
    else width=0;
    if (c==0) break;
    if (c=='.')
      {
      prec=atoi(format+1);
      while(isdigit(c=*++format));
      if (c==0) break;
      }
    else prec=0;
    if (prec) printf("printf: prec is not realized now :( ");
    /* обрабатываем modifier */
    /* % [FLAG] [WIDTH] [.PREC] [MODIFIER] TYPE */
    if (strchr("FNhlL",c=*format)!=NULL)
      {Modifier=c; format++;
      switch(Modifier)
        {
        case 'F':
        case 'N':
        case 'h':
        case 'l': break;
        case 'L':
          printf("printf: Modifier '%c' is invalid\n",Modifier);
        }
      }
    else Modifier=0;
    if (c==0) break;
    if (strchr("diouxXfeEgGcspn%",c=*format)==NULL)
      {Type=0; printf("printf: invalid type '%c'",c);}
    else
      {
      Type=c;
      /* вывод аргумента */
      switch(Type)
        {
        case '%': *str++='%'; counter++; break;
        case 'u': /* unsigned int */
        case 'i': /* signed int */
        case 'd': /* signed int */
                  if (Type=='u')
                    if (Modifier=='l') L=va_arg(v,unsigned long);
                    else L=va_arg(v,unsigned int);
                  else
                    if (Modifier=='l') L=va_arg(v,long);
                    else L=va_arg(v,int);

                  #ifdef DEBUG
                  printf("printf: L=%08lX\n",L);
                  #endif

                  if (Type!='u') if (L<0) {*str++='-'; width--; L=-L;}

                  if (Type=='u')
                    if (Modifier=='l') ultoa(L,new_printf_buf,10);
                    else ultoa(L,new_printf_buf,10);
                  else
                    if (Modifier=='l') ltoa(L,new_printf_buf,10);
                    else itoa((int)L,new_printf_buf,10);

                  #ifdef DEBUG
                  printf("printf: new_printf_buf=%s\n",new_printf_buf);
                  #endif
                  len=(int)strlen(new_printf_buf);
                  /* подавляем незначащие нули */
                  if (L)
                    {
                    ss=new_printf_buf;
                    if (*ss=='-')ss++;
                    while(*ss=='0')*ss++=' ';
                    fcc=ss;
                    }
                  else
                    fcc="0";
                  goto l_s;
        case 'x':
        case 'X': if (Modifier=='l')
                    {
                    if (width==0) width=8;
                    ltoa(va_arg(v,long),new_printf_buf,16);
                    }
                  else
                    {
                    if (width==0) width=4;
                    itoa(va_arg(v,int),new_printf_buf,16);
                    }
                  if ((len=(int)strlen(new_printf_buf))>width)
                    {
                    fcc=new_printf_buf;
                    fcc+=len-width;
                    }
                  else
                    fcc=new_printf_buf;
                  goto l_s;
        case 'p': if (width<5)
                    {itoa(va_arg(v,int),new_printf_buf,10);
                    strcpy(str,new_printf_buf);
                    counter+=(i=(int)strlen(new_printf_buf));
                    str+=i;}
                  else
                    {
                    itoa(va_arg(v,int),new_printf_buf,10);
                    strcpy(str,new_printf_buf);
                    counter+=(i=(int)strlen(new_printf_buf));
                    str+=i;
                    *str++=':'; counter++;
                    itoa(va_arg(v,int),new_printf_buf,10);
                    strcpy(str,new_printf_buf);
                    counter+=(i=(int)strlen(new_printf_buf));
                    str+=i;
                    }
                  break;
        case 'c': *str++=(char)va_arg(v,int); counter++; break;
        case 'o': if (Modifier=='l') {ltoa(va_arg(v,long),new_printf_buf,8);}
                  else {itoa(va_arg(v,int),new_printf_buf,8);}
                  fcc=new_printf_buf;
                  goto l_s;
        case 's':
                  #if 1
                  if (Modifier=='F')
                    {fcc=va_arg(v, char far *);
                    if (fcc==(char far *)NULL) fcc="(null)";
                    }
                  else
                    {cc=va_arg(v,char *);
                    if (cc==(char *)NULL) fcc="(null)";
                    else
                      {
                      #ifdef BOOT /* compile printf for Boot Manager */
                        asm mov ax,DS; /* SS ? */
                        asm mov segment,ax;
                        fcc = MK_FP (segment, cc);
                      #else /* Compile printf for Proolix Kernel */
                      if (User)
                        {
                        asm mov ax,ProcessPar;
                        asm mov segment,ax;
                        fcc = MK_FP (segment, cc);
                        }
                      else
                        {
                        asm mov ax,DS; /* SS ? */
                        asm mov segment,ax;
                        fcc = MK_FP (segment, cc);
                        }
                      #endif
                      }
                    }
                  #else
                    fcc=va_arg(v, char far *);
                    if (fcc==(char far *)NULL) fcc="(null)";
                  #endif
                  #ifdef DEBUG
                  puts("printf: fcc="); puts(fcc);
                  #endif
                  l_s:
                  i=(int)strlen(fcc);
                  if (width>i)
                    {
                    if (FlagMinus)
                      {
                      strcpy(str,fcc);
                      memset(str+i,' ',width-i);
                      }
                    else
                      {int j;char c;
                      if (Type=='s') c=' ';
                      else if (FlagZero) c='0';
                      else c=' ';
                      memset(str,c,j=width-i);
                      strcpy(str+j,fcc);
                      }
                    str+=width; counter+=width;
                    }
                  else
                    {
                    strcpy(str,fcc);
                    counter+=i; str+=i;
                    }
                  break;
        case 'f':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
        case 'n':
        default: printf("printf: type '%c' not supported in this realisation",
                        Type);
        }
      }
    }
  format++;
  }
*str=0;
return counter;
}
/****************************************************************************/
int sprintf(char far *str, const char far *format, ...)
{va_list v;
va_start(v,0);
return vsprintf(str, format, v);
}
/****************************************************************************/
int vprintf(const char far *format, va_list v)
{
int counter;
/* char str[MAX_LEN_STR]; */
char new_printf_buf[MAX_LEN_STR]; /* см. примечание в начале ф-ции printf */

va_start(v,0);
counter=vsprintf(new_printf_buf,format,v);
if(Puts0(new_printf_buf)==EOF)return EOF;
return counter;
}
/****************************************************************************/
