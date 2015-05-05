#include <prool.h>

#include <conf.c>
#include "kernel.h"
/****************************************************************************/
/* #define DEBUG */
/****************************************************************************/
char *DirToPath(struct dirent far *Dir, char *path)
{char c, far *cc, *ret; int i; unsigned len;
#ifdef DEBUG
printf("D2P: dir=%s ",(*Dir).filename);
#endif
if (Dir==NULL) return NULL;
if (path==NULL) return NULL;
ret=path;
if ((*Dir).attrib.B.Attr1)
  {
  cc=memchr((*Dir).filename,' ',11);
  if (cc==NULL) len=11; else len=(int)(cc-(*Dir).filename);
  memcpy(path,(*Dir).filename,(len>11)?11:len);
  path[len]=0;
  }
else
  {
  for(i=0;i<8;i++)
    if ((c=(*Dir).filename[i])!=' ') *path++=c;
    else break;
  if ((*Dir).filename[8]!=' ')
    {
    *path++='.';
    for (i=8;i<11;i++)
      if ((c=(*Dir).filename[i])!=' ') *path++=c;
      else break;
    }
  *path=0;
  strlwr(ret);
  }
#ifdef DEBUG
printf(" path=%s ",ret);
#endif
return ret;
}