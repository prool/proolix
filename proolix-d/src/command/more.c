/* more - command from OS Proolix
(C) Serge Pustovoitoff, <prool@infocom.kharkov.ua>
07.11.95

make for Proolix

        make more

make for MSDOS

        tcc more

*/

/* сюда надо добавить листание на экран по пробелу и на строку по вводу */

#include <stdio.h>
#include <string.h>

#ifdef Proolix
#include <limits.h>
#else
#define MAX_LINE 24
#endif

#define STR_LEN 256

void main (int argc, char *argv[])
{char str [STR_LEN];
FILE far *ff;
int l, nl=1, counter=0;

if (argc!=2)
  puts("Usage: more filename");
else
  {
  if ((ff=fopen(argv[1],"r"))==NULL) printf("more: Can't open %s\n",argv[1]);
  else
    while (fgets(str,STR_LEN,ff)!=NULL)
      {
      l=(int)strlen(str);
      if (str[l-1]=='\n') str[l-1]=0;
      /* ohs(str); */
      #ifdef Proolix
      if (str[0]==10) puts(str+1); else puts(str);
      #else
      puts(str);
      #endif
      counter++;
      if (++nl>MAX_LINE)
        {
        printf("%s, %i-%i",argv[1],counter-(MAX_LINE-1),counter);
        #ifdef Proolix
        if (getchar()=='q') break;
        #else
        if (getch()=='q') break;
        #endif
        puts("");
        nl=1;
        }
      }
  }
}
