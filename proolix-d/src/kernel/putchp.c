int putch (int);

/* putchp() - print of printable characters */

int putchp(int c)
{
switch (c)
  {
  case 0: c=' '; break;
  default: if (c<' ') c='.';
  }
return putch(c);
}