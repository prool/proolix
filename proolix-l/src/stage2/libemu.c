#define MAXLEN 256

void videomod(unsigned short int ax)
{
	return;
}

short int get_boot_drive(void) {return 0xAAAA;}

short int GetDriveParam_bx (char drive) {if (drive==0) return 4; else return 0xFFFF;}
short int GetDriveParam_cx (char drive) {if (drive==0) return 0x4F12; else return 0;}
short int GetDriveParam_dx (char drive) {if (drive==0) return 0x0101; else return 1;}

int kbhit(void) {return 0;}

unsigned short int end_of (void) {return 0;}

unsigned short int get_sp (void) {return 0;}

short int get_rtc(void)
{
time_t timer;
struct tm *timetable;
timer=time(0);
timetable = localtime(&timer);

#if 0
printf("debug time day %i month %i year %i hour %i min %i sec %i \n",
timetable->tm_mday,
timetable->tm_mon+1,
timetable->tm_year+1900,
timetable->tm_hour,
timetable->tm_min,
timetable->tm_sec
);
#endif

return ((((timetable->tm_hour/10)<<4) | (timetable->tm_hour%10))<<4 | (timetable->tm_min/10) )<<4 | (timetable->tm_min%10);
}

short int get_day(void)
{
time_t timer;
struct tm *timetable;
timer=time(0);
timetable = localtime(&timer);
return ((((timetable->tm_mday/10)<<4) | (timetable->tm_mday%10))<<4|((timetable->tm_mon+1)/10) )<<4|((timetable->tm_mon+1)%10);
}

short int get_year(void)
{
time_t timer;
struct tm *timetable;
int year, d1, d2, d3, d4;

timer=time(0);
timetable = localtime(&timer);

year=1900+timetable->tm_year;
d4=year%10;
d3=(year/10)%10;
d2=(year/100)%10;
d1=(year/1000)%10;

return (((d1<<4) | d2)<<4 | d3)<<4 | d4;
}

void reboot (void) {return;}
void cold (void) {return;}
void hdd0 (void) {return;}
void hdd1 (void) {return;}
void fdd (void) {return;}
void basic(void) {return;}

void putch_tty(char c) {putchar(c);}

unsigned short int readsec0(char drive, char sec, char head, char trk /* or cyl */, char *Buffer) {return 0;}
unsigned short int writesec0(char drive, char sec, char head, char trk /* or cyl */, char *Buffer) {return 0;}

void from_host (void) // copy file from host machine to disk image
{
char host_file [MAXLEN];
char virtual_file [MAXLEN];
char *cc;
char c;
int i, fi, fo;

printf("From host file ? ");
fgets(host_file, MAXLEN, stdin);
cc=strchr(host_file,'\n');
if (cc) *cc=0;
//printf("Host file '%s'\n", host_file);

printf("To virtual file ? ");
//for (i=0;i<MAXLEN;i++) virtual_file[i]=0;
fgets(virtual_file, MAXLEN, stdin);
cc=strchr(virtual_file,'\n');
if (cc) *cc=0;
printf("Virtual file '%s'\n", virtual_file);

fi=open(host_file,O_READ);
if (fi==-1) {printf("Can't open host file '%s'\n",host_file); return;}

fo=open_(virtual_file,O_CREAT);
if (fo==-1) {printf("Can't create virtual file '%s'\n",virtual_file); return;}

while(1)
	{
	if (read(fi,&c,1)!=1) {break;}
	if (writec(fo,c)==-1) {printf("Can't write virtual file '%s'\n",virtual_file); break;}
	}

close(fi);
if (close_(fi)==-1) {printf("Can't close virtual file '%s'\n",virtual_file); return;}
}

void to_host (void)
{
}
