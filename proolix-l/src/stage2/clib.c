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

void help(void)
{
puts0("Proolix-l shell command\r\n\r\n\
test - test\r\n\
help - help\r\n\
ascii - write ascii table\r\n\
cls - clearscreen\r\n\
palette - print color palette\r\n\
exit, quit - exit\r\n\
");
}

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

void putch3(char c)
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
	putch3(*s++);
//	putch(*s++);
}


void test (void)
{int i,j;

setpos(0,0);

puts0("stroka 1\r\n");
puts0("stroka 2\r\n");                                                                                                       
puts0("stroka 3\r\n");

//for (i=0;i<256;i++) {global_color=i; putch3('W');}

//putch3('a');

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
	c=getch(); putch3(c);
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

void putdec (int w)
{int r,i,trail_zeroes;
trail_zeroes=1;
int Divisor10 [] = {10000, 1000, 100, 10, 1};

if (w<0) {putch('-'); w=-w;}
if (w==0) {putch('0'); return;}

            for (i=0;i<5;i++)
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

#if 0
void puts0(char *s)
{
    while (*s)
	putch(*s++);
}
#endif

void putch(char c)
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
/****************************************************************************/

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

void out_boot(void *buf)

{int i;
unsigned long DiskSize;
unsigned long TrueSectors;
struct BootStru *b;
//b=buf; ///////////////////////////////////////////
puts0("Jump command \r\n");
#if 0
puthex_b((*b).Jmp[0]);
puthex_b((*b).Jmp[1]);
puthex_b((*b).Jmp[2]);

puts0("OEM name     ");
puts0((*b).OEM);

for(i=0;i<8;) printf("%02X ",(*b).OEM[i++]);

printf("\nSector size                      %4i bytes Cluster size                %1i sect\n",(*b).SectSiz,(*b).ClustSiz);
printf("Reserved sectors (before 1st FAT)  %2i       FAT counter                 %1i\n",(*b).ResSecs,(*b).FatCnt);
printf("Root directory entries           %4i       Total sectors         %7u\n",(*b).RootSiz,(*b).TotSecs);
printf("Media descr                        %02X       FAT size                %5i sect\n",(*b).Media,(*b).FatSize);
printf("Track size                         %2i sec   Heads                     %3i\n",(*b).TrkSecs,(*b).HeadCnt);
printf("Hidden sectors                %7li       Big Number Of Sectors %7li\n",(*b).HidnSec,(*b).BigSect);
printf("Physical Drive No                  %02X       ",(*b).DriveNo);
printf("Reserved byte              %02X \n",(*b).Thing);
printf("Extended Boot Signature            %02X       ",(*b).BootSign);
printf("Volume Serial No %04X-%04X",
(*b).SerialNo[1],
(*b).SerialNo[0]);

printf("\nVolume Label (in boot)  ");
for(i=0;i<11;)putchp((*b).VolLbl[i++]);
printf("\n                        ");
for(i=0;i<11;)printf("%02X ",(*b).VolLbl[i++]);
printf("\nFile system Id          ");
for(i=0;i<8;)putchp((*b).FileSysId[i++]);
printf("\n                        ");
for(i=0;i<8;)printf("%02X ",(*b).FileSysId[i++]);

if ((*b).TotSecs==0) TrueSectors=(*b).BigSect;
else TrueSectors=(*b).TotSecs;

DiskSize=((long) (*b).SectSiz * TrueSectors)/1024l;

if (DiskSize>5000)
  {
  DiskSize/=1024;
  printf("\nDisk size %li Mb\n",DiskSize);
  }
else
  {
  printf("\nDisk size %li Kb\n",DiskSize);
  }
#endif
}
