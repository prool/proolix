#include <prool.h>

#include <conf.c>
#include "kernel.h"

/****************************************************************************/
void far *memcpy(void far *dest, const void far *src, size_t n)
{size_t i; void far *ret;
/* if (dest==NULL) return NULL; */
/* if (src==NULL) return NULL; */
ret=dest;
for (i=0;i<n;i++) *((char far *)dest)++=*((char far *)src)++;
return ret;
}
/****************************************************************************/
void far *memccpy(void far *dest, const void far *src, int c, size_t n)
{size_t i;
/* if (dest==NULL) return NULL; */
/* if (src==NULL) return NULL; */
for (i=0;i<n;i++)
  {
  *(((char far *)dest)++)=*((char far *)src);
  if (*(((char far *)src)++)==c) {return (char far *)src;}
  }
return NULL;
}
/****************************************************************************/
int memcmp (const void far *s1, const void far *s2, size_t n)
{char c1,c2; size_t i;

/* if (s1==NULL) return 0; */
/* if (s2==NULL) return 0; */
for(i=0;i<n;i++)
  {
  c1=*((char far *)s1+(unsigned int)i);
  c2=*((char far *)s2+(unsigned int)i);
  if (c1>c2) return 1;
  else if (c1<c2) return -1;
  }
return 0;
}
/****************************************************************************/
int memicmp (const void far *s1, const void far *s2, size_t n)
{char c1,c2; size_t i;

/* if (s1==NULL) return 0; */
/* if (s2==NULL) return 0; */
for(i=0;i<n;i++)
  {
  c1=toupper(*((char far *)s1+(unsigned int)i));
  c2=toupper(*((char far *)s2+(unsigned int)i));
  if (c1>c2) return 1;
  else if (c1<c2) return -1;
  }
return 0;
}
/****************************************************************************/
void far *memchr (const void far *str, int c, size_t n)
{size_t i;
/* if (str==NULL) return NULL; */
for(i=0;i<n;i++)
  if (*((char far *)str+(unsigned int)i)==c)
    return (char far *)str+(unsigned int)i;
return NULL;
}
/****************************************************************************/
void far *memset  (void far *str, int c, size_t n)
{size_t i;
/* if (str==NULL) return NULL; */
for(i=0;i<n;i++) *((char far *)str+(unsigned int)i)=c;
return str;
}
