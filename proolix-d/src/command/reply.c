int reply(char *str)
{
int c,i;
  while (1)
  {c=getche() & 0xdf;
  for (i=0; str[i]!='\0';)
    if ((char)c==str[i++])
      {putch(13);putch(10);return i;}
  putch(7);
  }
}
