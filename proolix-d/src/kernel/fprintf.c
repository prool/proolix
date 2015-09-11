#include <stdarg.h>

#include <prool.h>

#include <conf.c>
#include "kernel.h"
/****************************************************************************/
int fprintf(FILE far *stream, const char far *format, ...)
{
va_list v; int counter;
char buf [MAX_LEN_STR];

va_start(v,0);
counter=vsprintf(buf,format,v);
if(fputs(buf, stream)==EOF) return EOF;
return counter;
}
/****************************************************************************/
int vfprintf(FILE *stream, const char *format, va_list v)
{
int counter;
char buf [MAX_LEN_STR];

va_start(v,0);
counter=vsprintf(buf,format,v);
if(fputs(buf,stream)==EOF)return EOF;
return counter;
}
/****************************************************************************/
