/* Man utility for Proolix by Yurii Rashkovskii
  ( 2:461/11@fidonet.org, yrashk@antisocial.com  )
  27.04.98 # Started (1st release)

  man 0.1

 Thanks to Prool <prool@itl.net.ua> for his more.c sources.

*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
#define MAN "/usr/man/"
#define MAX_LINE 24
#define STR_LEN 256
#define Proolix

char man_d[120];

void getman(char *prog);
void main(int argc,char *argv[])
{char str[120];

 FILE far *fp;

 printf("man 0.1 utility, Copyright (c) 1998, Yurii Rashkovskii <yrashk@antisocial.com>\n");
  if (argc==1) getman("man"); else
 {
 strcpy(str,MAN);
 strcat(str,argv[1]);
 fp=fopen(str,"r");
 if (fp==NULL) {
 setcolor(0x0F);
 printf("man: Unable to open %s (help file)\nPlease type 'man man' to get help\n",str);
 setcolor(0x07);
 } else {
 fgets(man_d,1,fp);
 man_d[29]=0;
 fclose(fp);
 getman(argv[1]);
 }
 }
}

void getman(char *prog)
{
{char str [STR_LEN];
char st[120];

FILE far *ff;
int l, z,nl=1, counter=0;
 strcpy(st,MAN);
 strcat(st,prog);
  printf("man: listing %s%s\n",MAN,prog);
  if ((ff=fopen(st,"r"))==NULL)
    {printf("man: Can't open %s\n",prog);}
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
        printf("  %% ( [ %s ] [%i] of [%i]  [ ESC exits ])",man_d,counter-(MAX_LINE-1),counter);
        if (getchar()==27) { printf("\n"); break; }
        puts("");
        nl=1;
        for (z=1;z<25;z++) printf("\n");
        }
      }
  }

}