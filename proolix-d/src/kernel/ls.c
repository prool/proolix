#include <prool.h>

#include <conf.c>
#include "kernel.h"
/****************************************************************************/
/* #define DEBUG */
/****************************************************************************/

#define ZASTAVKA_LL "     Name        Attr           Aux             Time      Date  Clu      Len\n\
-------------   --------- -------------------- -------- -------- ----- ----------\n"
#define ZASTAVKA_L "     Name        Attr      Time     Date    Clu     Len\n\
-------------   -------- -------- -------- ----- ----------\n"

void l_help(void)
{
puts("Uses: l [-l] [-L] [-U] [-H] [-F] [-T] [-M]\n\n\
Switches:\n\n\
-l - long info\n\
-L - very long info\n\
-U - list unused dir records\n\
-H - hex output (default - dec output)\n\
-F - free space\n\
-T - title\n\
-M - more");
}
/****************************************************************************/
void OutAttr(struct AttrField far *Attr)
{

if ((*Attr).Attr1)    putch('*'); else putch('.');
if ((*Attr).Attr2)    putch('*'); else putch('.');
if ((*Attr).Archive)  putch('A'); else putch('.');
if ((*Attr).Dir)      putch('D'); else putch('.');
if ((*Attr).Label)    putch('L'); else putch('.');
if ((*Attr).System)   putch('S'); else putch('.');
if ((*Attr).Hidden)   putch('H'); else putch('.');
if ((*Attr).ReadOnly) putch('R'); else putch('.');
}
/****************************************************************************/
void OutTimeDate(struct ftime far *t)
{
printf("%2i:%02i:%02i %2i-%02i-%02i",
(*t).ft_hour, (*t).ft_min, (*t).ft_tsec*2,
(*t).ft_day, (*t).ft_month,1980+(*t).ft_year);
}
/****************************************************************************/
void OutDate(struct fdate far *t)
{
printf("%2i-%02i-%02i",
(*t).ft_day, (*t).ft_month,1980+(*t).ft_year);
}
/****************************************************************************/
int l (int argc, char far *argv[])
{int i, j, k, n;
struct dirent far *D;
char Path [NAME_MAX+1];
int FlagL=0;
int FlagU=0;
int FlagLL=0;
int FlagH=0;
int FlagF=0;
int FlagT=0;
unsigned long size;
unsigned long SumSize=0;
unsigned long Total;
DIR far *dir;

argc--;

i=1;
do
  {
  if (argc)
    {
    if (argv[i][0]=='-')
      {
      n=(int)strlen(argv[i]);
      for (j=1;j<n;j++)
        switch (argv[i][j])
          {
          case 'l': FlagL++;           break;
          case 'L': FlagLL++; FlagL++; break;
          case 'U': FlagU++;           break;
          case 'H': FlagH++;           break;
          case 'M': More=2; NLine=1;   break;
          case 'F': FlagF++;           break;
          case 'T': FlagT++;           break;
          default : l_help();          return 1;
          }
      }
    else
      {/* Files */
      }
    }
  }
while (i++<argc);

if (FlagT)
  if (FlagL)
    if (FlagLL) printf(ZASTAVKA_LL); else printf(ZASTAVKA_L);

if ((dir=opendir("."))==NULL) {perror("l: "); return 1; }

#ifdef DEBUG
printf("ls:1");
#endif

while(1)
  {
  if ((D=(void far *)readdir(dir))==NULL) break;
  #ifdef DEBUG
  printf("ls:R");
  #endif
  if ((*D).attrib.B.Label && !FlagLL) continue;
  if (!(*D).filename[0] || ((*D).filename[0]==0xE5))
    {
    if (FlagU) if (printf("<Unused>\n")==EOF) goto l_ret;
    continue;
    }
  printf("%-13s",DirToPath((void far *)D,Path));
  SumSize+=(size=(*D).Size);
  if (!FlagL)
    {if (puts("")==EOF) goto l_ret; continue; }
  else
    {
      {
      OutAttr(&((*D).attrib.B));
      OutTimeDate(&((*D).FileUpdateDateTime));
      printf(FlagH?" %8lX":" %10li",(*D).FirstClu);
      if (printf(FlagH?" %8lX\n":" %10lu\n",size)==EOF) goto l_ret;
      }
    }
  }
l_ret:
if (closedir(dir)==EOF) perror("l: closedir");

if (FlagF)
  {
  Total=(MaxSectors - (unsigned long)DataStart) * (unsigned long)SECTOR_SIZE;
  printf("Total drive size %li\n",Total);

  printf("Total allocated  %li\n",SumSize);

  printf("Free             %li\n",Total-SumSize);
  }
More=0; NLine=1;
return 0;
}