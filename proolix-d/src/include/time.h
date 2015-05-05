/*      time.h

        Struct and function declarations for dealing with time.

        Copyright (c) Serge Pustovoitoff 1994-1997

*/

#ifndef _TM_DEFINED
#define _TM_DEFINED

#ifndef  __TIME_T
#define  __TIME_T
typedef long    time_t;
#endif

#ifndef  __CLOCK_T
#define  __CLOCK_T
typedef long clock_t;
#endif

struct  tm      {
        int     tm_sec;
        int     tm_min;
        int     tm_hour;
        int     tm_mday;
        int     tm_mon;
        int     tm_year;
        int     tm_wday;
        int     tm_yday;
        int     tm_isdst;
};

char    * asctime (const struct tm *tblock);
char far* ctime   (time_t time); /* prool */
double    difftime(time_t time2, time_t time1);
struct tm * gmtime(const time_t *timer);
struct tm * localtime(const time_t *timer);
time_t    time    (time_t far *timer); /* proolix */
clock_t  clock(void);

#if !__STDC__
extern int       daylight;
extern long      timezone;

int      stime(time_t *tp);
void     tzset(void);
#endif

#endif
