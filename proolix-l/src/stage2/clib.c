// proolix-l c library

#if 0
void two_parameters(int *i, int *j) // see translated text in .s file
{
*i=getch();
*j=getch();
}
#endif

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

char  *strcpy (char  *dest, const char  *src)
{char  *cc;
/* if (dest==NULL) return NULL; */
/* if (src==NULL) return NULL; */
cc=dest;
  do *dest++ = *src;
  while (*src++);
return cc;
}
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
size_t strlen (const char *s)
{
size_t i;
/* if (s==NULL) return NULL; */
i=0;
while (*s++)i++;
return i;
}
/****************************************************************************/

char *strchr (const char *str, int c)
{
/* if (str==NULL) return NULL; */
while (*str)
  if (*str==c)
    return (char *) str;
  else str++;
return NULL;
}
/****************************************************************************/

int tolower (int ch)
{
if ((ch>='A')&&(ch<='Z'))return ch + 'a' - 'A'; else return ch;
}
/****************************************************************************/
int toupper (int ch)
{
if ((ch>='a')&&(ch<='z'))return ch + 'A' - 'a'; else return ch;
}

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
