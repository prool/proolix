/* help - command from OS Proolix

by Serge Pustovoitoff, <prool@itl.net.ua>

13.06.2004
26.05.2002

*/

/* todo:
сюда надо добавить листание на экран по пробелу и на строку по вводу */

#include <stdio.h>
#include <conio.h>
#include <string.h>

#ifdef Proolix
#include <limits.h>
#else
#define MAX_LINE 24
#endif

#define STR_LEN 90

#define HELPFILE "/usr/man/help.doc"

void main (void)
{char str [STR_LEN];
FILE far *ff;
int l, nl=1, counter=0;

  if ((ff=fopen(HELPFILE,"r"))==NULL) printf("more: Can't open %s\n",HELPFILE);
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
        printf("%s, %i-%i",HELPFILE,counter-(MAX_LINE-1),counter);
        if (getch()=='q') {puts (""); break; }
        puts("");
        nl=1;
        }
      }
}