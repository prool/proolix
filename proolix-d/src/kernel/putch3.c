#include <prool.h>

#include <conf.c>
#include "kernel.h"

int putch3 (int ch)
{
int LastRowOffset, i;
switch(ch)
  {
  case 0:    break;
  case '\n': curX=1;
             curY++;
             curAddr=MK_FP(VIDEO_SEG,maxX*(curY-1)*2);
             if (curY>maxY) goto scroll;
             break;
  default:
             *curAddr++=ch;
             *curAddr++=VideoAttrib;
             if (++curX>maxX)
               {
               curX=1;
               if (++curY>maxY)
                 {scroll:
                 curY=maxY;
                 curAddr-=maxX*2;
                 LastRowOffset=(maxY-1)*maxX*2;
                 memcpy(MK_FP(VIDEO_SEG,0),MK_FP(VIDEO_SEG,maxX*2),LastRowOffset);
                 for (i=0;i<maxX*2;)
                   {
                   *(char far *)MK_FP(VIDEO_SEG,LastRowOffset+i++)=' ';
                   *(char far *)MK_FP(VIDEO_SEG,LastRowOffset+i++)=7;
                   }
                 }
               }
  }
return ch;
}

