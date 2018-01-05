#include "headers.h"

int strcmp (const char  *s1, const char  *s2)
{char c1,c2;

/* if (s1==NULL) return 0; */
/* if (s2==NULL) return 0; */
do  {c1=*s1++; c2=*s2++; if (c1>c2) return 1; else if (c1<c2) return -1;}
while (c1&&c2);
return 0;
}
