/*-------------------------------------------------------------------------*/
/*                                 external cat                            */
/*-------------------------------------------------------------------------*/

/* #define DEBUG */

#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

#include "prool.h"

int FlagH=0;
int FlagA=0;
int FlagT=0;
int FlagN=0;
int Flag_s=1;
int Col=0;
int Line=0;

void help(void)
{
puts("Uses: cat [-sHMATN] [-Ooffset] [files...]\n\
Flags:\n\
-H - hex output\n\
-M - more\n\
-A - hex & ascii output\n\
-T - address titles\n\
-N - output to /dev/null\n\
-Ooffset - hex offset\n\
-s - don't print error messages");
}

#ifdef MSDOS
long int htol(char *s)
{
sscanf(s,"%X",htol);
}
#endif

int main(int argc, char *argv[] )
{
unsigned char c[512];
int i, ii, j, h, n, Files=0;
unsigned long offset=0;
unsigned long adr=0;

argc--;

ii=1;
do
  {
  if (argc)
    {
    if (argv[ii][0]=='-')
      {
      n=(int)strlen(argv[ii]);
      for (j=1;j<n;j++)
        switch (argv[ii][j])
          {
          case 'O': offset=htol(argv[ii]+2);
                    j=n; /* это для выхода из цикла for */
                    break;
          case 'H': FlagH++;  break;
          case 'M': More=2; break;
          case 'A': FlagA++;  break;
          case 'T': FlagT++;  break;
          case 'N': FlagN++;  break;
          case 's': Flag_s=0; break;
          default : help(); return 1;
          }
      }
    else
      {
      Files++;
      if ((h=open(argv[ii],O_RDONLY))==-1)
        if (Flag_s) printf("Can't open %s\n",argv[ii]);

      if (FlagT && Flag_s) printf("offset = %x\n",offset);
      if (offset)
        {
        for (i=0; i<offset/512; i++)
          {
          n=(int)read(h,c,512);
          if (n!=512) if (Flag_s) puts("Can't skip offset");
          }
        n=(int)read(h,c,j=(int)(offset%512l));
        if (n!=j) if (Flag_s) puts("Can't skip offset");
        adr= (unsigned long int) offset;
        offset=0;
        }

      while (1)
        {
        if((n=(int)read(h,c,16))<=0) break;
        if(FlagN) continue;
        if (FlagT) printf("%8lX  ",adr);
        adr+=16;
        if (FlagH+FlagA)
          {
          for (i=0;i<n;i++) printf("%2X ",c[i]);
          if (n<16)
            for(i=0;i<(16-n);i++) printf("   ");
          if (FlagA)
            {
            putchar (' ');
            for (i=0;i<n;i++)
              {
              if (c[i]>=' ') {if (putchar(c[i])==EOF) goto L_close; }
              else putchar(' ');
              }
            }
          if (puts("")==EOF) goto L_close;
          }
        else
          for (i=0;i<n;i++) if (putchar(c[i])==EOF) goto L_close;
        }
      L_close:
      close(h);
      #ifdef DEBUG
      printf("\ncat: close: finished\n");
      #endif
      }
    }
  }
while (ii++<argc);

#ifdef DEBUG
printf("\ncat: breakpoint A\n");
#endif

/*
if (!Files)
  вывод файла /dev/tty
  */

More=0;
return 0;
}
