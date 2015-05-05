int i;
char buf[MAX_LEN_STR];
int c;

if (s==NULL) {errno=EINVAL; return NULL;}

for(i=0;i<MAX_LEN_STR;)
  {
  c=getchar();
  #ifdef DEBUG
  putch('<');
  putch(c);
  putch('>');
  #endif
  switch (c)
    {
    case '\b' : /* ERASE=8 */
                if (i)
                  {
                  if (wherex()==0)
                    {int y;
                    y=wherey();
                    if (y!=0) y--;
                    buf[--i]=0;
                    gotoxy(79,y);
                    putch(' ');
                    gotoxy(79,y);
                    }
                  else
                    {
                    buf[--i]=0;
                    putch('\b');
                    putch(' ');
                    putch('\b');
                    }
                  }
                else
                  buf[i]=0;
                break;
    case EOF  :
    case   4  : /* CTRL D */
    case '\r' : /* CR */
    case '\n' : /* LF */
                buf[i]=0; i=MAX_LEN_STR; break;
    default:    buf[i++]=c;
    }
  }

buf[MAX_LEN_STR-1]=0;

#ifdef DEBUG
printf(" gets:buf=",buf);
puts(buf);
#endif

#if 1
for (i=0;i<MAX_LEN_STR;i++)
  {
  *s++=buf[i];
  if (buf[i]==0) break;
  }
#endif

#ifdef DEBUG
printf(" gets:s=");
puts(s);
#endif

return s;
