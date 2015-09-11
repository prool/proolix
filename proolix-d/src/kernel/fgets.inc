int i;
char str [MAX_LEN_STR];
int c;

if (s==NULL) goto l_err;

if (n>MAX_LEN_STR)
  {printf("fgets: n>MAX_LEN_STR"); l_err:errno=EINVAL;return NULL;}

for(i=0;i<MAX_LEN_STR-2;)
  {
  c=fgetc(stream);
  switch (c)
    {
    case EOF  : if (i==0) {s[0]=0; return NULL; }
    case '\r' : /* CR */
                str[i]=0; goto L;
    default:    str[i++]=c;
    }
  }
L:
str[MAX_LEN_STR-1]=0;
strcpy(s,str);
return s;
