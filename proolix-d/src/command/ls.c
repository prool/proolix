#include <limits.h>

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <dirent.h>

#include <prool.h>
char *DirToPath(struct dirent far *Dir, char *path);
/****************************************************************************/
void help(void)
{
puts("Uses: ls [-l] [-L] [-U] [-H] [-T] [-M] [-S]\n\n\
Switches:\n\n\
-l - long info\n\
-L - very long info\n\
-U - list unused dir records\n\
-H - hex output (default - dec output)\n\
-T - title\n\
-S - type summa of filesizes\n\
-M - more");
}
/****************************************************************************/
#if 0 /* сохранено для истории */
void OutAttr(char a)
{char Letter [] = "ADLSHR";
char Mask;
int i;

Mask=0x20;
for (i=0;i<6;i++){if(a&Mask)putch(Letter[i]); else putch('.');Mask>>=1;}
}

void OutTime(unsigned int t)
{
printf("%2i:%02i:%02i",t>>11,(t&0x7ff)>>5,
(unsigned int)(t & (unsigned int) 0x1f)*2);
}

void OutDate(unsigned int t)
{
printf("%2i-%02i-%02i",t&0x1f,(t&0x1e0)>>5,1980+(t>>9));
}
#endif
/****************************************************************************/

/****************************************************************************/
#if 1
void OutAttr(struct AttrField Attr)
{

if (Attr.Attr1)    putch('*'); else putch('.');
if (Attr.Attr2)    putch('*'); else putch('.');
if (Attr.Archive)  putch('A'); else putch('.');
if (Attr.Dir)      putch('D'); else putch('.');
if (Attr.Label)    putch('L'); else putch('.');
if (Attr.System)   putch('S'); else putch('.');
if (Attr.Hidden)   putch('H'); else putch('.');
if (Attr.ReadOnly) putch('R'); else putch('.');
}

void OutTimeDate(struct ftime t)
{
printf("%2i:%02i:%02i %2i-%02i-%02i", t.ft_hour, t.ft_min, t.ft_tsec*2,
t.ft_day, t.ft_month,1980+t.ft_year);
}
#endif
/****************************************************************************/
int main (int argc, char *argv[])
{int i, j, k, n;
long l;
struct dirent far *D;
char Path [NAME_MAX+1];
int FlagL=0;
int FlagU=0;
int FlagLL=0;
int FlagH=0;
int FlagT=0;
int FlagS=0;
unsigned long size;
unsigned long SumSize=0;
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
          case 'T': FlagT++;           break;
          case 'S': FlagS++;           break;
          default : help();            return 1;
          }
      }
    else
      {/* Files */
      }
    }
  }
while (i++<argc);

if (FlagT)
  {
  if (FlagL)
    if (FlagLL)
      {
printf(
"     Name        Attr           Aux             Time      Date  Clu      Len\n\
-------------   --------- -------------------- -------- -------- ----- ----------\n");
      }
    else
      {
printf(
"     Name        Attr      Time     Date    Clu     Len\n\
-------------   -------- -------- -------- ----- ----------\n");
      }
  }


if ((dir=opendir("."))==NULL) {perror("ls: opendir"); return 1;}
for(l=0;;l+=32)
  {
  if ((D=readdir(dir))==NULL) break;
  if ((*D).attrib.B.Label && !FlagLL) continue;
  if (!(*D).filename[0] || ((*D).filename[0]==0xE5))
    {
    if (!FlagU) goto l_cont;
    }
  printf("%-13s",DirToPath(D,Path));
  SumSize+=(size=(*D).Size);
  if (!FlagL)
    {if (puts("")==EOF) goto l_ret;
    goto l_cont; }
  else
    {
    OutAttr((*D).attrib.B);
    putch(' ');
    OutTimeDate((*D).FileUpdateDateTime);
    printf(FlagH?" %4lX":" %6lu",(*D).FirstClu);
    if (printf(FlagH?" %8lX\n":" %7lu\n",size)==EOF)
      {l_ret: closedir(dir); return 1; }
    }
  l_cont: ;
  }
if (closedir(dir)==EOF) perror("ls: closedir");

if (FlagS) printf("Total allocated  %li\n",SumSize);

More=0; NLine=1;
return 0;
}