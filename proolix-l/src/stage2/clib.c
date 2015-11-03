// proolix-l c library

#include <limits.h>

 // for printf
#define MAX_LEN_STR 256 
#define EOF (-1)

#if 0
void two_parameters(int *i, int *j) // see translated text in .s file
{
*i=getch();
*j=getch();
}
#endif

int toupper (int ch)
{
if ((ch>='a')&&(ch<='z'))return ch + 'A' - 'a'; else return ch;
}

char *strchr (const char *str, int c)
{
/* if (str==NULL) return NULL; */
while (*str)
  if (*str==c)
    return (char *) str;
  else str++;
return NULL;
}

size_t strlen (const char *s)
{
size_t i;
/* if (s==NULL) return NULL; */
i=0;
while (*s++)i++;
return i;
}

char  *strcpy (char  *dest, const char  *src)
{char  *cc;
/* if (dest==NULL) return NULL; */
/* if (src==NULL) return NULL; */
cc=dest;
  do *dest++ = *src;
  while (*src++);
return cc;
}

void cls(void)
{
cls_();
setpos(0,0);
}

void putch(char c)
{char g_col,g_row;
g_row=get_row();
g_col=get_col();
switch(c)
	{
	case '\r': g_col=0; setpos(g_row,g_col); break;
	case '\n': if (g_row==24)
			scroll();
		   else
			{g_row++; setpos(g_row,g_col);}
		   break;
	default:
		putch_color(c,get_color());
		g_col++;
		setpos(g_row,g_col);
	}
}

void puts0(char *s)
{
    while (*s)
	putch(*s++);
//	putch(*s++);
}


void test (void)
{int i,j;

setpos(0,0);

puts0("stroka 1\r\n");
puts0("stroka 2\r\n");                                                                                                       
puts0("stroka 3\r\n");

//for (i=0;i<256;i++) {global_color=i; putch('W');}

//putch('a');

#if 0
setpos(0,0);putch_color('1',2);
setpos(0,1);putch_color('2',3);
setpos(0,2);putch_color('3',4);
#endif
}

void ascii(void)
{int i;
for (i=0;i<256;i++) putch(i);
}

char *getsn(char *str, int len)
{char c; int i;
i=0;
while(1)
	{
	c=getch(); putch(c);
	if (c==0x0DU) {*str=0; return str;}
	*str=c;
	if (++i>=(len-1)) {*str=0; return str;}
	str++;
	}
}

void puthex1(char c)
{
if (c>9) putch('A'+c-10);
else putch('0'+c);
}

void puthex_b(char c)
{
puthex1((c>>4)&0xFU);
puthex1(c&0xFU);
}

void puthex(int c)
{
puthex_b((c>>8) & 0xFFU);
puthex_b(c&0xFFU);
}

void puthex_l(int c)
{
puthex ((c>>16)&0xFFFFU);
puthex (c & 0xFFFFU);
}

void putdec (int w)
{int r,i,trail_zeroes;
trail_zeroes=1;
int Divisor10 [10] = {1000000000,100000000,10000000,1000000,100000,10000, 1000, 100, 10, 1};

if (w<0) {putch('-'); w=-w;}
if (w==0) {putch('0'); return;}

            for (i=0;i<10;i++)
              {
              r=w/Divisor10[i];
              w%=Divisor10[i];
              if (r!=0)
            	    {
            	    putch('0'+r);
            	    trail_zeroes=0;
            	    }
              else // r==0
            	    if (trail_zeroes)
            		{// nothing
            		}
            	    else
            		{
            		putch('0');
            		}
              }
}

void putdec2 (int w, int digits, int zeroes)
{int r,i,trail_zeroes,j;
trail_zeroes=1;
int Divisor10 [10] = {1000000000,100000000,10000000,1000000,100000,10000, 1000, 100, 10, 1};

if ((digits<1)||(digits>10)) return;

if (w<0) {putch('-'); w=-w;}
if (w==0) {putch('0'); return;}

            for (i=10-digits;i<10;i++)
              {
              r=w/Divisor10[i];
              w%=Divisor10[i];
              if (r>9)
        	    {// overflow
        	    for (j=0;j<digits;j++) putch('*');
        	    return;
        	    }
              if (r!=0)
            	    {
            	    putch('0'+r);
            	    trail_zeroes=0;
            	    }
              else // r==0
            	    if (trail_zeroes)
            		{if (zeroes==0) putch('0');
			else putch(' ');
            		}
            	    else
            		{
            		putch('0');
            		}
              }
}

#if 0
void puts0(char *s)
{
    while (*s)
	putch(*s++);
}
#endif

void putch0(char c)
{
/*
VIDEO - TELETYPE OUTPUT

AH = 0Eh
AL = character to write
BH = page number
BL = foreground color (graphics modes only)
*/
__asm__ volatile("movb $0x0E,%ah");
__asm__ volatile("movb %0,%%al"::"r"(c):"%al");
__asm__ volatile("xorb %bh,%bh");
__asm__ volatile("int $0x10");

}

void stop(void)
{
__asm__ volatile("int $0x20");
}

int getch(void)
{
/*
AH = 00h

Return:
AH = BIOS scan code
AL = ASCII character
*/
__asm__ volatile("xor %ah,%ah");
__asm__ volatile("int $0x16");
__asm__ volatile("xor %ah,%ah");
}

int strcmp (const char  *s1, const char  *s2)
{char c1,c2;

/* if (s1==NULL) return 0; */
/* if (s2==NULL) return 0; */
do  {c1=*s1++; c2=*s2++; if (c1>c2) return 1; else if (c1<c2) return -1;}
while (c1&&c2);
return 0;
}

#if 0 // запас функций, которые пока не используются

/****************************************************************************/
char  *stpcpy (char  *dest, const char  *src)
{
/* if (dest==NULL) return NULL; */
/* if (src==NULL) return NULL; */
  do *dest++ = *src;
  while (*src++);
return dest-1;
}
/****************************************************************************/
size_t strcspn(const char  *s1, const char  *s2)
{
size_t i;

/* if (s1==NULL) return NULL; */
/* if (s2==NULL) return NULL; */
i=0;
while(*s1)
  {
  if (strchr(s2,*s1)!=NULL)break;
  i++;
  s1++;
  }
return i;
}
/****************************************************************************/
char  *strcat (char  *dest, const char  *src)
{
char  *cc;
/* if (dest==NULL) return NULL; */
/* if (src==NULL) return NULL; */
cc=dest;
while(*dest++);
strcpy(--dest,src);
return cc;
}
/****************************************************************************/
int stricmp(const char  *s1, const char  *s2)
{char c1,c2;

/* if (s1==NULL) return 0; */
/* if (s2==NULL) return 0; */
do
  {
  c1=*s1++;
  c2=*s2++;
  if (isalpha(c1) && isalpha(c2))
    {
    c1=toupper(c1);
    c2=toupper(c2);
    }
  if (c1>c2) return 1;
  else if (c1<c2) return -1;
  }
while (c1&&c2);
return 0;
}
/****************************************************************************/
char  *strlwr(char  *s)
{
char  *cc;
/* if (s==NULL) return NULL; */
cc=s;
while(*s)
  {
  *s=tolower(*s);
  s++;
  }
return cc;
}
/****************************************************************************/
char  *strncat (char  *dest, const char  *src, size_t maxlen)
{
char  *cc;
/* if (dest==NULL) return NULL; */
/* if (src==NULL) return NULL; */
cc=dest;
while(*dest++);
strncpy(--dest,src,maxlen);
return cc;
}
/****************************************************************************/
int strncmp(const char  *s1, const char  *s2, size_t maxlen)
{char c1,c2;
size_t i;

/* if (s1==NULL) return 0; */
/* if (s2==NULL) return 0; */
for(i=0;i<maxlen;i++)
 {
 c1=*s1++;
 c2=*s2++;
 if (c1>c2) return 1;
 else if (c1<c2) return -1;
 if(!(c1&&c2))break;
 }
return 0;
}
/****************************************************************************/
char  * strncpy (char  * dest, const char  * src, size_t maxlen)
{char  * cc; size_t i;
/* if (dest==NULL) return NULL; */
/* if (src==NULL) return NULL; */
cc=dest;
for(i=0;(i<maxlen);i++) {*dest++=*src; if (!*src++) break;}

return cc;
}
/****************************************************************************/
int strnicmp(const char  *s1, const char  *s2, size_t maxlen)
{char c1,c2;
size_t i;

/* if (s1==NULL) return 0; */
/* if (s2==NULL) return 0; */
for(i=0;i<maxlen;i++)
 {
 c1=*s1++;
 c2=*s2++;
 if (isalpha(c1)&&isalpha(c2))
   {
   c1=toupper(c1);
   c2=toupper(c2);
   }
 if (c1>c2) return 1;
 else if (c1<c2) return -1;
 if(!(c1&&c2))break;
 }
return 0;
}
/****************************************************************************/
char  *strnset(char  *s, int c, size_t n)
{size_t i;
char  *ret;
ret=s;
/* if (s==NULL) return NULL; */
for(i=0;i<n;i++)
  {
  if (!*(s+(unsigned int)i)) break;
  *s=c;
  }
return ret;
}
/****************************************************************************/
char  *strpbrk(const char  *s1, const char  *s2)
{
/* if (s1==NULL) return NULL; */
/* if (s2==NULL) return NULL; */
while(*s1)
  {
  if (strchr(s2,*s1)!=NULL) break;
  s1++;
  }
return (char  *)s1;
}
/****************************************************************************/
char  *strrchr(const char  *s, int c)
{
char  *cc;
/* if (s==NULL) return NULL; */
cc=NULL;
while(*s)
  {
  if (*s==c) cc=(char  *)s;
  s++;
  }
return cc;
}
/****************************************************************************/
char  *strset(char  *s, int c)
{char  *cc;
/* if (s==NULL) return NULL; */
cc=s;
while(*s)
  {
  *s=c;
  s++;
  }
return cc;
}
/****************************************************************************/
size_t strspn(const char  *s1, const char  *s2)
{
size_t i;
/* if (s1==NULL) return NULL; */
/* if (s2==NULL) return NULL; */
i=0;
while(*s1)
  {
  if (strchr(s2,*s1)==NULL)break;
  i++;
  s1++;
  }
return i;
}
/****************************************************************************/
char  *strstr(const char  *s1, const char  *s2)
{
size_t l;
/* if (s1==NULL) return NULL; */
/* if (s2==NULL) return NULL; */
l=strlen(s2);
while(*s1)
  {
  if (!strncmp(s1,s2,l))return (char  *)s1;
  s1++;
  }
return NULL;
}
/****************************************************************************/
static char  *strtok_ptr;
/****************************************************************************/
char  *strtok(char  *s1, const char  *s2)
{
char  *cc;

if (s1==NULL) if((s1=strtok_ptr)==NULL) return NULL;

/* Пропуск символов-заполнителей в начале строки */
while(*s1)
  {
  if (strchr(s2,*s1)==NULL)break;
  s1++;
  }
if(!*s1)return NULL;
cc=s1; /* это начало токена */
/* Поиск конца токена */
while(*s1)
  {
  if (strchr(s2,*s1)!=NULL) break;
  s1++;
  }
if (*s1) {*s1=0; strtok_ptr=s1;} else strtok_ptr=NULL;
return cc;
}
/****************************************************************************/
char  *strupr(char  *s)
{
char  *cc;
/* if (s==NULL) return NULL; */
cc=s;
while(*s)
  {
  *s=toupper(*s);
  s++;
  }
return cc;
}
/****************************************************************************/
char  * strrev  (char  *str)
{size_t i1, i2; char Bolvan,  *ret;
ret=str;
/* if (str==NULL) return NULL; */
i1=1;
i2=(int)strlen(str);
while(i1<i2)
  {
  /* swap str[i1] str[i2] */
  /* Стандартная схема пересадки при помощи болвана
  (C) А.Мирер, "Дом скитальцев" */
  Bolvan=*(str+(unsigned int)i1);
  *(str+(unsigned int)i1++)=*(str+(unsigned int)i2);
  *(str+(unsigned int)i2--)=Bolvan;
  }
return ret;
}

/****************************************************************************/

int tolower (int ch)
{
if ((ch>='A')&&(ch<='Z'))return ch + 'a' - 'A'; else return ch;
}
/****************************************************************************/

/****************************************************************************/
int isgraph(int c)
{
return ((c) >= 0x21 && (c) <= 0x7e);
}
/****************************************************************************/
int isprint(int c)
{
return ((c) >= 0x20 && (c) <= 0x7e);
}
/****************************************************************************/
int isalnum (int c) {return isdigit(c)||isalpha(c);}
int isalpha (int c) {return islower(c)||isupper(c);}
int iscntrl (int c) {return c<' ';}
int isdigit (int c) {return (c>='0')&&(c<='9');}
int islower (int c) {return (c>='a')&&(c<='z');}
int ispunct (int c)
{
switch(c)
  {
  /*  (C)  Список символов пунктуации взят из программы Connect Д.Орлова и др.
  Copyright by KHEMZ, Inc. 1991, 1994 */
  case '.':
  case ',':
  case '!':
  case '"':
  case '\'':
  case '(':
  case ')':
  case '*':
  case '+':
  case '-':
  case '/':
  case ':':
  case ';':
  case '<':
  case '=':
  case '>':
  case '?':
  case '[':
  case '\\':
  case ']':
  case '^':
  case '{':
  case '|':
  case '}':
  case '~': return 1;
  default:  return 0;
  }
}
/****************************************************************************/
int isspace (int c) {if ((c==' ')||(c=='\t')) return 1; else return 0;}
int isupper (int c) {return (c>='A')&&(c<='Z');}
int isxdigit(int c)
  {return ((c>='0')&&(c<='9'))||((c>='a')&&(c<='f'))||((c>='A')&&(c<='F'));}

#endif

/****************************************************************************/
void  *memcpy(void  *dest, const void  *src, size_t n)
{size_t i; void  *ret;
/* if (dest==NULL) return NULL; */
/* if (src==NULL) return NULL; */
ret=dest;
for (i=0;i<n;i++)
	{
	*((char  *)dest)=*((char  *)src);
	(char *)dest++;
	(char *)src++;
	}
return ret;
}
/****************************************************************************/
void  *memccpy(void  *dest, const void  *src, int c, size_t n)
{size_t i;
/* if (dest==NULL) return NULL; */
/* if (src==NULL) return NULL; */
for (i=0;i<n;i++)
  {
  *((char  *)dest)=*((char  *)src);
  (char *)dest++;
  if (*((char  *)src)==c) {return (char  *)src;}
  (char *)src++;
  }
return NULL;
}
/****************************************************************************/
int memcmp (const void  *s1, const void  *s2, size_t n)
{char c1,c2; size_t i;

/* if (s1==NULL) return 0; */
/* if (s2==NULL) return 0; */
for(i=0;i<n;i++)
  {
  c1=*((char  *)s1+(unsigned int)i);
  c2=*((char  *)s2+(unsigned int)i);
  if (c1>c2) return 1;
  else if (c1<c2) return -1;
  }
return 0;
}
/****************************************************************************/
int memicmp (const void  *s1, const void  *s2, size_t n)
{char c1,c2; size_t i;

/* if (s1==NULL) return 0; */
/* if (s2==NULL) return 0; */
for(i=0;i<n;i++)
  {
  c1=toupper(*((char  *)s1+(unsigned int)i));
  c2=toupper(*((char  *)s2+(unsigned int)i));
  if (c1>c2) return 1;
  else if (c1<c2) return -1;
  }
return 0;
}
/****************************************************************************/
void  *memchr (const void  *str, int c, size_t n)
{size_t i;
/* if (str==NULL) return NULL; */
for(i=0;i<n;i++)
  if (*((char  *)str+(unsigned int)i)==c)
    return (char  *)str+(unsigned int)i;
return NULL;
}
/****************************************************************************/
void  *memset  (void  *str, int c, size_t n)
{size_t i;
/* if (str==NULL) return NULL; */
for(i=0;i<n;i++) *((char  *)str+(unsigned int)i)=c;
return str;
}

#if 0 // printf package

int printf (const char  *format, ...)
{
va_list v; int counter;
char new_printf_buf[MAX_LEN_STR];

/* Called functions: vsprintf(), puts0() */

/* Использование в качестве буфера массива printf_buf, выделяемого по
malloc (см. в kernel.c) вызывает глюки (зависания).
А использование в качестве буфера
локального массива new_printf_buf глюков не вызывает. Все работает. */

va_start(v,0);
counter=vsprintf(new_printf_buf,format,v);
puts0(new_printf_buf);
return counter;
}
/****************************************************************************/
int vsprintf (char  *str, const char  *format, va_list v)
{
int i, counter=0, width, prec, Modifier, Type,
  FlagMinus, FlagPlus, FlagBlank, FlagDiez, FlagZero;
char c, *cc,  *fcc;
unsigned int segment;
int len; char  *ss; long L;
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
                  #if 0 // 1 - proolix-d, 0 -proolix-l
                  if (Modifier=='F')
                    {fcc=va_arg(v, char  *);
                    if (fcc==(char  *)NULL) fcc="(null)";
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
                    fcc=va_arg(v, char  *);
                    if (fcc==(char  *)NULL) fcc="(null)";
                  #endif
                  #ifdef DEBUG
                  puts0("printf: fcc="); puts0(fcc);
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
int sprintf(char  *str, const char  *format, ...)
{va_list v;
va_start(v,0);
return vsprintf(str, format, v);
}

#endif // printf package

#if 0
int vprintf(const char  *format, va_list v)
{
int counter;
/* char str[MAX_LEN_STR]; */
char new_printf_buf[MAX_LEN_STR]; /* см. примечание в начале ф-ции printf */

va_start(v,0);
counter=vsprintf(new_printf_buf,format,v);
if(puts0(new_printf_buf)==EOF)return EOF;
return counter;
}
#endif

#if 0 // global variables NOT PERMIT!!!!!!!!!
char HexDigit [] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
int Divisor8  [] = {0100000, 010000, 01000, 0100, 010, 1};
long Divisor10L [] = {1000000000L,
                      100000000L,
                       10000000L,
                        1000000L,
                         100000L,
                          10000L,
                           1000L,
                            100L,
                             10L,
                              1L};

long Divisor8L [] = {010000000000L,
                     01000000000L,
                      0100000000L,
                       010000000L,
                        01000000L,
                         0100000L,
                          010000L,
                           01000L,
                            0100L,
                             010L,
                              01L};

#endif

int htoi(const char  *s)
{
int i; char c;
i=0;
while (*s)
  {
  i<<=4;
  c=toupper(*s++);
  if ((c>='0')&&(c<='9')) i+=c-'0';
  else if ((c>='A')&&(c<='F')) i+=c+10-'A';
  else return 0;
  }
return i;
}
/****************************************************************************/
long htol(const char  *s)
{
long i; char c;
i=0;
while (*s)
  {
  i<<=4;
  c=toupper(*s++);
  if ((c>='0')&&(c<='9')) i+=c-'0';
  else if ((c>='A')&&(c<='F')) i+=c+10-'A';
  else return 0;
  }
return i;
}
/****************************************************************************/
int atoi(const char  *s)
{
int i; char c;
i=0;
if (*s=='+') s++;
else if(*s=='-')return-atoi(++s);
while (*s)
  {
  c=*s++;
  if (isdigit(c)) {i=i*10; i+=c-'0';}
  else break;
  }
return i;
}
/****************************************************************************/
long atol(const char  *s)
{
long i; char c;
i=0;
if (*s=='+') s++;
else if(*s=='-')return-atol(++s);
while (*s)
  {
  c=*s++;
  if (isdigit(c)) {i=i*10; i+=c-'0';}
  else break;
  }
return i;
}
/****************************************************************************/
#if 0
void fuck1(void)
{
int Divisor [] = {10000, 1000, 100, 10, 1};

printf("fuck1 = %04X\n",Divisor[0]);

}
#endif

#if 0 // itoa package
char  *itoa (int w, char  *str, int radix)
{
char  *ret;
int i, r, power, n;
int Divisor10 [] = {10000, 1000, 100, 10, 1};

/* printf("itoa = %04X\n",Divisor10[0]); */

if (radix>34) radix=34;

ret=str;
if (w<0) if (radix==10) {*str++='-'; itoa(-w,str,-10); return ret;}

switch(radix)
  {
  case   2: for (i=0;i<16;i++) {*str++='0'+ w & 0x8000; w<<=1;} break;
  case   8: for (i=0;i<6;i++)
              {r=w/Divisor8[i]; w%=Divisor8[i]; *str++='0'+r;}
            break;
  case  16: for (i=0;i<4;i++) {*str++=HexDigit[(w&0xF000)>>12];w<<=4;} break;
  case  10: *str++='0';
            for (i=0;i<5;i++)
              {r=w/Divisor10[i]; w%=Divisor10[i]; *str++='0'+r;
              #ifdef DEBUG
              printf("itoa: Divisor10[i]=%04X r=%04X w=%04X\n",
                Divisor10[i],r,w);
              #endif
              }
            break;
  case -10: for (i=0;i<5;i++)
              {r=w/Divisor10[i]; w%=Divisor10[i]; *str++='0'+r;
              }
            break;
  default:  power=UINT_MAX;
            for (i=1;i<16;i++) {power/=radix; if (power>0) break;}
            n=i; power=1;
            for (i=1;i<n;i++,power*=radix);
            for (i=0;i<n;i++)
              {power/=radix; r=w/power; w%=power; *str++=HexDigit[r];}
            break;
  }
*str=0;
return ret;
}
/****************************************************************************/
char  *ltoa (long w, char  *str, int radix)
{
char  *ret;
int i, n;
long r, power;

if (radix>34) radix=34;

ret=str;
if (w<0) if (radix==10) {*str++='-'; ltoa(-w,str,-10); return ret;}

switch(radix)
  {
  case  2: for (i=0;i<32;i++) {*str++='0'+ w & 0x8000; w<<=1;} break;
  case  8: for (i=0;i<11;i++)
             {r=w/Divisor8L[i]; w%=Divisor8L[i]; *str++='0'+r;}
           break;
  case 16: for (i=0;i<8;i++)
             {*str++=HexDigit[(int)((w&0xF0000000L)>>28)];w<<=4;}
           break;
  case 10: *str++='0';
           for (i=0;i<10;i++)
             {r=w/Divisor10L[i]; w%=Divisor10L[i]; *str++='0'+r;
             }
           break;
  case -10: for (i=0;i<10;i++)
              {r=w/Divisor10L[i]; w%=Divisor10L[i]; *str++='0'+r;
              }
            break;
  default: power=ULONG_MAX;
           for (i=1;i<32;i++) {power/=radix; if (power>0) break;}
           n=i; power=1;
           for (i=1;i<n;i++,power*=radix);
           for (i=0;i<n;i++)
             {power/=(long)radix; r=w/power; w%=power; *str++=HexDigit[(int)r];}
           break;
  }
*str=0;
return ret;
}
/****************************************************************************/
char  *ultoa (unsigned long w, char  *str, int radix)
{
char  *ret;
int i, n;
long r, power;

if (radix>34) radix=34;

ret=str;

switch(radix)
  {
  case  2: for (i=0;i<32;i++) {*str++='0'+ w & 0x8000; w<<=1;} break;
  case  8: for (i=0;i<11;i++)
             {r=w/Divisor8L[i]; w%=Divisor8L[i]; *str++='0'+r;}
           break;
  case 16: for (i=0;i<8;i++)
             {*str++=HexDigit[(int)((w&0xF0000000L)>>28)];w<<=4;}
           break;
  case 10: *str++='0';
           for (i=0;i<10;i++)
             {r=w/Divisor10L[i]; w%=Divisor10L[i]; *str++='0'+r;
             }
           break;
  case -10: for (i=0;i<10;i++)
              {r=w/Divisor10L[i]; w%=Divisor10L[i]; *str++='0'+r;
              }
            break;
  default: power=ULONG_MAX;
           for (i=1;i<32;i++) {power/=radix; if (power>0) break;}
           n=i; power=1;
           for (i=1;i<n;i++,power*=radix);
           for (i=0;i<n;i++)
             {power/=(long)radix; r=w/power; w%=power; *str++=HexDigit[(int)r];}
           break;
  }
*str=0;
return ret;
}
#endif // itoa package

void process_boot (void *buf)
{int i;
  struct BootStru *b;
  b=buf;
  HeadCnt=b->HeadCnt;
  TrkSecs=b->TrkSecs;
  SectorsOnCyl=HeadCnt*TrkSecs;

  if ((i=b->ResSecs)!=0) ResSecs=i; ResSecs=b->HidnSec+1;
  if ((i=b->TotSecs)!=0) MaxSectors=i;
  else MaxSectors=b->BigSect+b->HidnSec;

  CluSize=b->ClustSiz;
  CluSizeBytes=CluSize * 512;
  FatSize=b->FatSize;
  RootBeg = ResSecs + 2 * b->FatSize; // 2 - FatCnt
  RootEnd = RootBeg + ( ( b->RootSiz * 32 ) / b->SectSiz ) - 1;
  DataStart = RootEnd+1;
  MaxClusters=((MaxSectors-DataStart)/CluSize+1);
//  puts0("process_boot end\r\n");
}

void ls (void)
{
char buf[512];
unsigned char *pp;
int i,j,k, sec;
struct dirent16 *d;

puts0("RootBeg=");putdec(RootBeg);
puts0("\r\nFilename    Flags    Date                Size 1stClu 1stSector\r\n");

for(i=RootBeg;i<RootEnd;i++)
    {
    for (k=0;k<512;k++) buf[k]=0;
    secread(0,i,buf);
    pp=buf;
    for (j=0;j<16;j++)
	{
	d=(void *)pp;
	if ((*pp==0x0E5)||(*pp==0)||(d->AttrByte&8)) {pp+=32; continue;}
	for (k=0;k<11;k++) putch(d->d_name[k]);
	putch(' ');
#if 1
	if (d->AttrByte&128)	putch('*'); else putch('.');
	if (d->AttrByte& 64)	putch('*'); else putch('.');
	if (d->AttrByte& 32)	putch('A'); else putch('.');
	if (d->AttrByte& 16)	putch('D'); else putch('.');
	if (d->AttrByte&  8)	putch('L'); else putch('.');
	if (d->AttrByte&  4)	putch('S'); else putch('.');
	if (d->AttrByte&  2)	putch('H'); else putch('.');
	if (d->AttrByte&  1)	putch('R'); else putch('.');
#endif

// time 0000 0000 0000 0000
//      hhhh hmmm mmms ssss
// date 0000 0000 0000 0000
//      yyyy yyym mmmd dddd

	putch(' ');
	putdec2((d->FileDate&0x0001FU),2,0); // day
	putch('-');
	putdec2((d->FileDate&0x001E0U)>>5,2,0); // month
	putch('-');
	putdec2(((d->FileDate&0x0FE00U)>>9)+1980,4,0); // year

	putch(' ');
	putdec2((d->FileTime&0x0F800U) >> 11,2,0); // heur
	putch(':');
	putdec2((d->FileTime&0x007E0U) >> 5,2,0); // min
	putch(':');
	putdec2((d->FileTime&0x0001FU) * 2,2,0); // sec
	putch(' ');
	putdec2(d->Size,5,1); // filesize
	putch(' ');
	putdec2(sec=((d->d_fileno)&0xFFFFU),4,1); // 1st cluster
	putch(' ');
	putdec(SecForClu(sec)); // 1st sector
	puts0("\r\n");
	pp+=32;
	}
    }
}

void out_boot(void *buf)
{int i;
unsigned long DiskSize;
unsigned long TrueSectors;
struct BootStru *b;
b=buf;
puts0("Jump command ");
puthex_b((*b).Jmp[0]);
puthex_b((*b).Jmp[1]);
puthex_b((*b).Jmp[2]);

puts0("\r\nOEM name     ");
puts0((*b).OEM);

for(i=0;i<8;) {puthex_b((*b).OEM[i++]); putch(' ');}
puts0("\r\nSector size                      ");putdec((*b).SectSiz);puts0(" bytes Cluster size                ");putdec((*b).ClustSiz);
puts0(" sect\r\n");
puts0("Reserved sectors (before 1st FAT)  ");putdec((*b).ResSecs);puts0("       FAT counter                 ");putdec((*b).FatCnt);
puts0("\r\nRoot directory entries           ");putdec((*b).RootSiz);puts0("       Total sectors               ");putdec((*b).TotSecs);
puts0("\r\nMedia descr                       ");puthex_b((*b).Media);puts0("       FAT size                    ");putdec((*b).FatSize);
puts0("\r\nTrack size                        ");putdec((*b).TrkSecs);puts0(" sec   Heads                       ");putdec((*b).HeadCnt);
puts0("\r\nHidden sectors                     ");putdec((*b).HidnSec);puts0("       Big Number Of Sectors       ");putdec((*b).BigSect);
puts0("\r\nPhysical Drive No                 ");puthex_b(b->DriveNo);
//puts0(" Reserved byte              ");puthex_b((*b).Thing);
//puts0("\r\nExtended Boot Signature            ");puthex_b((*b).BootSign);
puts0("       Volume Serial No            ");puthex((*b).SerialNo[1]);putch('-');puthex((*b).SerialNo[0]);
puts0("\r\nVolume Label (in boot)  ");
for(i=0;i<11;)putch((*b).VolLbl[i++]);
puts0("\r\n                        ");
for(i=0;i<11;) puthex_b((*b).VolLbl[i++]);
puts0("\r\nFile system Id          ");
for(i=0;i<8;)putch((*b).FileSysId[i++]);
puts0("\r\n                        ");
for(i=0;i<8;)puthex_b((*b).FileSysId[i++]);
if ((*b).TotSecs==0) TrueSectors=(*b).BigSect;
else TrueSectors=(*b).TotSecs;
DiskSize=((long) (*b).SectSiz * TrueSectors)/1024l;

if (DiskSize>5000)
  {
  DiskSize/=1024;
  puts0("\r\nDisk size ");putdec(DiskSize);puts0(" Mb\r\n");
  }
else
  {
  puts0("\r\nDisk size ");putdec(DiskSize);puts0(" Kb\r\n");
  }

puts0("\r\n0 - boot sector; 1 - "); // %i - FAT\n%i-%i - root dir\n%i-%i - data area\n");
putdec(RootBeg-1);
puts0(" - FAT; ");
putdec(RootBeg);
puts0(" - ");
putdec(DataStart-1);
puts0(" - root dir; ");
putdec(DataStart);
puts0(" - ");
putdec(MaxSectors-1);
puts0(" - data area\r\n");
}

void system(void)
{
puts0("\r\nsize of int ");putdec(sizeof(int));
puts0("\r\nsize of long int ");putdec(sizeof(long int));
puts0("\r\nsize of short int ");putdec(sizeof(short int));
puts0("\r\nsize of char ");putdec(sizeof(char));
}

void memmap(void)
{
puts0("\
Memory map for Proolix-l (real mode)\r\n\
0 - 1FF  Vectors\r\n\
200 - 3FF free area (stack of boot sector)\r\n\
400 - 4FF ROM BIOS data\r\n\
600 =0060:0000 - load kernel address (see src/stage2/boot.S, KernelSeg constant)\r\n\
end of Kernel (CT) 0060:");
puthex(end_of());
puts0("\r\n07C00 =0:7C00 (=0070:7500) Boot sector (stage1)\r\n\
07E00                      Boot sector end\r\n\
30500 (=3050:0000) stage2 (see src/boot-sector/boots.S, stage2_seg constant)\r\n\
MemTop (f.e. 9FFFF)\r\n\
A0000 EGA (in graph modes)\r\n\
B0000 MDA, Hercules 1st videopage\r\n\
B8000 CGA, EGA, VGA 1st videopage (mode=3, symbol mode)\r\n\
C8000 Additional ROM modules (2K blocks)\r\n\
E0000 Bg  AT ROM BIOS\r\n\
EFFFF End AT ROM BIOS\r\n\
F6000 ROM Basic\r\n\
FE000 ROM BIOS, POST\r\n\
FFFF0 JMP - COLD REBOOT\r\n\
FFFF5 BIOS version/date (ASCII)\r\n\
FFFFE PC/XT/AT identefication byte\r\n\
");
}

void help(void)
{
puts0("Proolix-l shell command\r\n\r\n\
test - test\r\n\
help, ? - this help\r\n\
ascii - write ascii table\r\n\
cls - clearscreen\r\n\
palette - print color palette\r\n\
system - print system parameters\r\n\
memd0 - memory dump for extended processor mode\r\n\
memd - memory dump for real mode\r\n\
memmap - print memory map\r\n\
basic - call ROM BASIC (if exist)\r\n\
diskd0 - disk dump #1 (sector/head/track)\r\n\
diskd - disk dump #2 (absolute sector)\r\n\
ls - ls\r\n\
exit, quit - exit\r\n\
");
}

void memd0(void)
{char str[MAX_LEN_STR];
int a,i,line,c;

puts0("adr? ");
getsn(str,MAX_LEN_STR);
a=(htoi(str));
//puts0("\r\nAddress = ");
//puthex_l(a);

puts0("\r\n");

#define MEMD_STEP 16

for (line=0;line<23;line++)
    {
    puthex_l(a); putch(' ');
    for (i=0;i<MEMD_STEP;i++)
	{
	c=peek(a+i);
	puthex_b(c);
	putch(' ');
	}
    for (i=0;i<MEMD_STEP;i++)
	{
	c=(peek(a+i))&0xFFU;
	if (c<' ') putch('.');
	else putch(c);
	}
    a+=MEMD_STEP;
    puts0("\r\n");
    }	
}

void memd(void)
{
char c;
int i;
char Option = 'M';
short int segm, off;
char cmd [MAX_LEN_STR];

/* Adr0 = (char *) 0; */
segm = 0;
off = 0;

puts0("Use /? command for help\r\n");

while (1)
  {
  puthex(segm);
  putch(':');
  puthex(off);
  putch(' ');
  switch (Option)
    {
    case 'A': for(i=0;i<16;i++)
                            {
                                if ((c=peek2(segm,off+i))>=' ') putch(c);
                                else putch('.');
                            }
	break;
    case 'M':
    	for(i=0;i<16;i++){puthex_b(peek2(segm,off+i));putch(' ');}
    	for(i=0;i<16;i++)
                            {
                                if ((c=peek2(segm,off+i))>=' ') putch(c);
                                else putch('.');
                            }
	break;
    case 'H': for(i=0;i<16;i++){puthex_b(peek2(segm,off+i));putch(' ');}
                          break;
    }
  puts0("\r\n");
  putch(':');
  getsn(cmd, MAX_LEN_STR);

  i=0; while (cmd[i]) if (cmd[i]=='\r') {cmd[i]=0; break;} else i++;

  if (cmd[0]=='/')
    {
    switch (c=toupper(cmd[1]))
      {
      case 'Q': puts0("\r\n"); return;
      case 'A':
      case 'H':
      case 'M': Option=c; break;
      case 'S': segm=htoi(cmd+2); break;
      case 'O': off=htoi(cmd+2); break;
      case 'K': memmap(); break;
      case 'Z': poke(htoi(cmd+2),segm,off); break;
      case '?':puts0(
"Commands:\r\n\
/Q - Quit\r\n\
/A - Ascii mode\r\n\
/H - Hex mode\r\n\
/M - Mix (hex & ascii) mode\r\n\
/S<hex_value> - set Segment\r\n\
/O<hex_value> - set Offset\r\n\
/K - print memory map\r\n\
Enter - next string\r\n\
/Z<hex_value> - poke hex_value to memory\r\n"); break;
      default: puts0("Invalid command\r\n");
      }
    puts0("\r\n");
    }
  else if (!cmd[0]) {off+=16;}
  else ;
  }
}

void diskd0(void)
{char Buffer [512];
char str[MAX_LEN_STR];
int drive, sec, head, trk, i, ii, c, line;

puts0("drive? ");
getsn(str,MAX_LEN_STR);
drive=htoi(str);

puts0("\r\nsec? (1-...) ");
getsn(str,MAX_LEN_STR);
sec=htoi(str);

puts0("\r\nhead? ");
getsn(str,MAX_LEN_STR);
head=htoi(str);

puts0("\r\ntrk (cyl)? ");
getsn(str,MAX_LEN_STR);
trk=htoi(str);

puts0("\r\ndrive = ");
puthex(drive);
puts0(" sec = ");
puthex(sec);
puts0(" head = ");
puthex(head);
puts0(" trk = ");
puthex(trk);

for(i=0;i<512;i++) Buffer[i]=0;

i=readsec0(drive, sec, head, trk /* or cyl */, Buffer);

if (i==1) puts0(" OK");
else {
puts0(" err code = ");
puthex(i);
}
puts0("\r\n");

ii=0;
for (line=0;line<32;line++)
    {
    if (line==15) {puts0(" press any key "); getch();putch('\r');}
    for (i=0;i<MEMD_STEP;i++)
	{
	c=Buffer[ii++];
	puthex_b(c);
	putch(' ');
	}
    ii=ii-MEMD_STEP;
    for (i=0;i<MEMD_STEP;i++)
	{
	c=(Buffer[ii++])&0xFFU;
	if (c<' ') putch('.');
	else putch(c);
	}
    puts0("\r\n");
    }	
}

void diskd(void)
{char Buffer [512];
char str[MAX_LEN_STR];
int drive, sec, i, ii, c, line;

puts0("drive? ");
getsn(str,MAX_LEN_STR);
drive=htoi(str);

puts0("\r\nabs sec? ");
getsn(str,MAX_LEN_STR);
sec=atoi(str);

puts0("\r\ndrive = ");
puthex(drive);
puts0(" sec = ");
putdec(sec);

for(i=0;i<512;i++) Buffer[i]=0;

i=secread(drive, sec, Buffer);

if (i==1) puts0(" OK");
else {
puts0(" err code = ");
puthex(i);
}
puts0("\r\n");

ii=0;
for (line=0;line<32;line++)
    {
    if (line==15) {puts0(" press any key "); getch();putch('\r');}
    for (i=0;i<MEMD_STEP;i++)
	{
	c=Buffer[ii++];
	puthex_b(c);
	putch(' ');
	}
    ii=ii-MEMD_STEP;
    for (i=0;i<MEMD_STEP;i++)
	{
	c=(Buffer[ii++])&0xFFU;
	if (c<' ') putch('.');
	else putch(c);
	}
    puts0("\r\n");
    }	
}

int secread (int drive, unsigned AbsSec, char *Buffer)
{/* Read absolute sectors
Input:
drive (for int 13h Fn=2)
abs sec number
buffer

Output: return!=0 if error
*/
int Track, SecNoOnCyl, i;
char Head, SecOnTrk;

Track=(AbsSec/SectorsOnCyl); /*SectorsOnCyl=HeadCnt*TrkSecs,Track==Cyl */
SecNoOnCyl=(AbsSec%SectorsOnCyl);
Head=SecNoOnCyl/TrkSecs;
SecOnTrk=SecNoOnCyl%TrkSecs+1;
       /*
       2 bytes are combined to a word similar to INT 13:

        7 6 5 4 3 2 1 0  1st byte  (sector)
        | | -------------Sector offset within cylinder
        +-+--------------High order bits of cylinder #

        7 6 5 4 3 2 1 0  2nd byte  (cylinder)
        -----------------Low order bits of cylinder #
        */
if ((i=Track &0x0300)!=0)
  {
  SecOnTrk = (SecOnTrk & 0x3F) | (short int)(i>>2);
  }
return readsec0(drive, SecOnTrk, Head, Track, Buffer);
}

int SecForClu (int CluNo)
{unsigned long l;
/*
Sector=ResSecs + FatCnt*FatSize + (RootSiz*32)/SectSiz + ((CluNo-2)*ClustSiz)
       ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
       DataStart
*/
if ((CluNo>MaxClusters)||(CluNo<2))
  {
  #if 0
  printf("SecForClu: Invalid cluster number. CluNo=%u, MaxClusters=%u",
  CluNo,MaxClusters);
  #endif
  return -1;
  }
l=DataStart + ( (CluNo-2) * CluSize );
#ifdef DEBUG
printf("sec4clu: return %li ",l);
#endif
return l;
}
