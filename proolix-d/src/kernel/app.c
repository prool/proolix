#include <stdio.h>
#include <string.h>
#include <mem.h>

#define MAX_STR_LEN 256

int main(int argc, char *argv[])
{FILE *fi, *fo;
char s [MAX_STR_LEN];
int Status;
char *cc;

printf("APP - Assembler PreProcessor Version 0.0.0.0  9-Feb-95\n\
by Serge Pustovoitoff, 1995. prool@itl.net.ua, 2:461/35\n\
\n");

if (argc!=3) {printf("Syntax: app input_file output_file\n"); exit(0);}

fi=fopen(argv[1],"r");
if (fi==NULL) {printf("Can't open input file %s\n",argv[1]); exit(1);}

fo=fopen(argv[2],"w");
if (fo==NULL) {printf("Can't open output file %s\n",argv[2]); exit(1);}

Status=0;

  if (fputs("\tinclude\tdebug.mac\n", fo)==EOF)
    {printf("Can't write output file %s\n",argv[1]); goto Ex; }

while(1)
  {
  s[0]=0;
  if (fgets(s, MAX_STR_LEN, fi)==NULL) break;
  switch (Status)
    {
    case 0: /* начальное состояние автомата */
            if (!strcmp(s,"\tifndef\t??version\n")) {Status=1; continue; }
            break;
    case 1: /* нашли ifndef ?? version */
            if (!strcmp(s,"\tendif\n")) Status=0;
            continue;
    default: ;
    }
  /* filtr */
  cc=s;
  if (memcmp(s,"\t?debug\t",7)==0)
    {
    switch (s[8])
      {
      case 'S':
      case 'C':
      case 'E':
      case 'B':
                continue;
      case 'L':
      default : s[1]='\t';
                cc=s+1;
      }
    }
  else if (memcmp(s+5,"short",5)==0)
    {
    strcpy(s+5,s+11);
    }

  if (fputs(cc, fo)==EOF)
    {printf("Can't write output file %s\n",argv[1]); break; }
  }

Ex:
fclose(fi);
if (fclose(fo)==EOF) {printf("Can't close output file %s\n",argv[1]); exit(1);}
exit(0);
}
