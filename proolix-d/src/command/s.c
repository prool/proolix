#include <stdio.h>
#include <stdlib.h>

#define SIZE_S 254

void main(int argc, char *argv[])
{
int i, n, m;
FILE far *fp;
char s [SIZE_S];
char *cc;

if (argc<2) printf("usage: s <filename> [ string_no [string_count] ]\n");

fp=fopen(argv[1],"r");
if (fp==NULL) {perror("s: fopen"); return; }

if (argc>2)
  if (argv[2][0]=='$') n=htoi(argv[2]+1);
  else                 n=atoi(argv[2]);
else n=0;

if (argc>3)
  if (argv[3][0]=='$') m=htoi(argv[2]+1);
  else                 m=atoi(argv[2]);
else m=1;

if (n==0)
  {
  for(i=1;;i++)
    {
    if (fgets(s,SIZE_S,fp)==NULL) break;
    cc=s;
    if (cc[0]=='\n') cc++;
    printf("\n%i:0x%04X: %s",i,i,cc);
    }
  }
else
  {
  for (i=1;i<n;i++)
    {printf("\r%i:0x%04X",i,i); if (fgets(s,SIZE_S,fp)==NULL) break; }
  for(i=n;i<=(n+m-1);i++)
    {
    cc=s;
    if (cc[0]=='\n') cc++;
    printf("%i:0x%04X: %s",i,i,cc);
    if (fgets(s,SIZE_S,fp)==NULL) break;
    }
  }
fclose(fp);
}
