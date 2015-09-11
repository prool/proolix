#include <prool.h>

#include <conf.c>

#include "kernel.h"

#include <time.h>

/*==========================================================================*/
#ifdef CMOS
extern unsigned char hour, min, secund;
#else
extern unsigned long Tick;
unsigned long trash;
#endif

extern unsigned char century, year, month, day, flag;
/*==========================================================================*/
unsigned char unpack(unsigned char c)
{
return (c>>4)*10 + (c & 0xf);
}
/*==========================================================================*/

#ifdef CMOS
void gettime (struct time far * timep)
{
readRTC (&hour, &min, &secund, &flag);

(*timep).ti_min=unpack(min);
(*timep).ti_hour=unpack(hour);
(*timep).ti_sec=unpack(secund);
(*timep).ti_hund=0;
}
/*==========================================================================*/
void getdate (struct date far * datep)
{
readRTCdate (&century, &year, &month, &day);

(*datep).da_year=unpack(century)*100+unpack(year)-1980;
(*datep).da_day=unpack(day);
(*datep).da_mon=unpack(month);
}
/*==========================================================================*/
#else
/*==========================================================================*/
void gettime (struct time far * timep)
{
ReadTick(&Tick,&flag);
trash=(Tick*1000)/182;
(*timep).ti_hund=trash%100;
trash/=100;
(*timep).ti_sec=trash%60;
trash/=60;
(*timep).ti_min=trash%60;
(*timep).ti_hour=trash/60;
}
/*==========================================================================*/
void getdate (struct date far * datep)
{
ReadTick(&Tick,&flag);
(*datep).da_year=century*100+year-1980;
(*datep).da_day=day+flag;
(*datep).da_mon=month;
}
#endif

/*==========================================================================*/
time_t time (time_t far *tloc)
{struct time timep; struct date datep; time_t l;

gettime (&timep);
getdate (&datep);
l=dostounix(&datep, &timep);
if (tloc!=NULL) *tloc=l;
return l;
}
/*==========================================================================*/
#pragma warn -par
char far *ctime (time_t timer)
{
return"Day Jan 00 99:99:99 1900\n";
}
#pragma warn +par

