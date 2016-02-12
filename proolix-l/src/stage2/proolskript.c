// Proolskript interpreter

#define MAXLEN1	256

#define MAXSTACK	10
#define MAXLABEL	10
#define DEC	0
#define HEX	1

#ifndef MAXFAJL
#define MAXFAJL	512
#endif

void skript(void)
{
char buf[MAXLEN1];
int i, line, file, rcode, console=0, mode=DEC;
int l, number;
char *cc;
char buf1[MAXLEN1];
int stack[MAXSTACK];
int label[MAXLABEL];
int j;
char fayl [MAXFAJL];
int faylp;
int ii;

for (i=0;i<MAXSTACK;i++) stack[i]=0;
for (i=0;i<MAXLABEL;i++) label[i]=-1;
for (i=0;i<MAXFAJL;i++) fayl[i]=0;

while(1)
	{
	puts0("Filename (? for dir, enter for console) > ");
	getsn(buf,MAXLEN1);
	if (buf[0]=='?') ls();
	else	break;
	}

if (buf[0]==0) {console=1; puts("quit for quit");}
else if ((file=open(buf,0))==-1) {puts0("\r\nFile not found :("); return;}

puts0("\r\nProol Skript Interpterer v.2.1\r\n");
puts0("MAXFAJL = "); putdec(MAXFAJL); puts("\r\n");

l_begin: ;
for (ii=0;ii<MAXFAJL; ii++) fayl[ii]=0;
// read file to fayl
if (console==0)
{
faylp=0;
while(1)
	{
	j=readw(file,buf1,MAXLEN1);
	if (buf1[0])
		{
		if (buf1[0]=='#') continue;
		ii=0; while(buf1[ii]) fayl[faylp++]=buf1[ii++];
		fayl[faylp++]='\r';
		}
	else if (j==0) break;
	}
close(file);
}
else	{
#ifdef PROOLSKRIPT_IN_LINUX
	puts0("skript> ");
#else
	puts0("skript> ");
#endif
	getsn(fayl,MAXFAJL);
#ifdef PROOLSKRIPT_IN_LINUX
	//
#else
	puts0("\r\n");
#endif
	for (ii=0; ii<MAXFAJL; ii++) if (fayl[ii]==' ') fayl[ii]='\r';
	}

#if 0 // debug output
ii=0; while(fayl[ii]) {char c=fayl[ii++];if (c=='\r') {putchar('\r');putchar('\n');} else putchar(c);}

puts("debug mode. press any key or control C, Luc"); getchar();
#endif

// perviy prohod
faylp=0;
if (1/*console==0*/)
 {
 number=MAXLABEL;
 while(fayl[faylp])
	{
	//j=readw(file,buf1,MAXLEN1);
	ii=0; while(fayl[faylp]) {if (fayl[faylp]!='\r') buf1[ii++]=fayl[faylp++]; else break;} faylp++; buf1[ii]=0;
	if (buf1[0])
		{
		//puts0("1: ");puts(buf1); // debug
		if (isdigit(buf1[0])) number=atoi(buf1);
		else if (!strcmp(buf1,"label"))
			{
				if (number>=MAXLABEL) puts("MAXLABEL ERROR");
				else label[number]=faylp;
			}
		}
	}
 }
#if 0 // debug mode
puts("debug mode. press any key or control C, Luc"); getchar();
#endif

// vtoroy prohod

faylp=0;
while(1/*fayl[faylp]*/)
	{
    	if (1/*console==0*/) {
			// readw:
			ii=0; while(fayl[faylp]) {if (fayl[faylp]!='\r') buf[ii++]=fayl[faylp++]; else break;} faylp++; buf[ii]=0;
			} else {getsn(buf,MAXLEN1); puts("");}
	if (1)
		{// eval buf
		//puts0("2: "); puts(buf); // debug
		if ((buf[0]!='#')&&(buf[0]!=0)) 
			{
			if(buf[0]=='!') puts0(buf+1);
			else if (isdigit(buf[0]))
				{
				for (i=0;i<MAXSTACK-1;i++) stack[i]=stack[i+1];
				if (mode==DEC) stack[MAXSTACK-1]=atoi(buf);
				else stack[MAXSTACK-1]=htoi(buf);
				}
			else if (!strcmp(buf,"test")) puts("TEST OK");
			else if (!strcmp(buf,"newline")) puts("");
			else if (!strcmp(buf,"inputstring")) getsn(buf1,MAXLEN1);
			else if (!strcmp(buf,"outputstring")) puts0(buf1);
			else if (!strcmp(buf,"outputint"))
				if (mode==DEC) putdec(atoi(buf1)); else puthex_l(atoi(buf1));
			else if (!strcmp(buf,"push"))
				{
				for (i=0;i<MAXSTACK-1;i++) stack[i]=stack[i+1];
				if (mode==DEC) stack[MAXSTACK-1]=atoi(buf1);
				else stack[MAXSTACK-1]=htoi(buf1);
				}
			else if (!strcmp(buf,"swap"))
				{int bolvan;
				bolvan=stack[MAXSTACK-2];
				stack[MAXSTACK-2]=stack[MAXSTACK-1];
				stack[MAXSTACK-1]=bolvan;
				}
			else if (!strcmp(buf,"dup"))
				{
				for (i=0;i<MAXSTACK-1;i++) stack[i]=stack[i+1];
				}
			else if (!strcmp(buf,"peek"))
				{
				stack[MAXSTACK-1]=peek(stack[MAXSTACK-1]);
				}
			else if (!strcmp(buf,"peek2"))
				{
				stack[MAXSTACK-1]=peek2(stack[MAXSTACK-2],stack[MAXSTACK-1]);
				for (i=MAXSTACK-2;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"+"))
				{
				stack[MAXSTACK-1]=stack[MAXSTACK-2]+stack[MAXSTACK-1];
				for (i=MAXSTACK-2;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"-"))
				{
				stack[MAXSTACK-1]=stack[MAXSTACK-2]-stack[MAXSTACK-1];
				for (i=MAXSTACK-2;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"/"))
				{int div;
				div=stack[MAXSTACK-1];
				if (div==0) puts("DIVIDE BY ZERO");
				else stack[MAXSTACK-1]=stack[MAXSTACK-2]/div;
				for (i=MAXSTACK-2;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"%"))
				{
				stack[MAXSTACK-1]=stack[MAXSTACK-2]%stack[MAXSTACK-1];
				for (i=MAXSTACK-2;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"*"))
				{
				stack[MAXSTACK-1]=stack[MAXSTACK-2]*stack[MAXSTACK-1];
				for (i=MAXSTACK-2;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"."))
				{
				if (mode==DEC) putdec(stack[MAXSTACK-1]); else puthex_l(stack[MAXSTACK-1]);
				if (console) puts("");
				for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,".b"))
				{
				puthex_b(stack[MAXSTACK-1]);
				if (console) puts("");
				for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,".w"))
				{
				puthex(stack[MAXSTACK-1]);
				if (console) puts("");
				for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,".l"))
				{
				puthex_l(stack[MAXSTACK-1]);
				if (console) puts("");
				for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,".c"))
				{
				putch(stack[MAXSTACK-1]);
				for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"drop"))
				{
				for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"label"))
				{
				l=stack[MAXSTACK-1];
				if (l>=MAXLABEL) puts("MAXLABEL ERROR");
				else label[l]=faylp;
				for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"goto"))
				{
				l=stack[MAXSTACK-1];
				if (l>=MAXLABEL) puts("GOTO MAXLABEL ERROR");
				else faylp=label[l];
				for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"ifgoto"))
				{
				if (stack[MAXSTACK-1]!=0)
					{
					l=stack[MAXSTACK-2];
					if (l>=MAXLABEL) puts("IFGOTO MAXLABEL ERROR");
					else faylp=label[l];
					}
				for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1]; // drop
				for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1]; // drop
				}
			else if (!strcmp(buf,"loop"))
				{int c, l;
				l=stack[MAXSTACK-1];
				c=stack[MAXSTACK-2]-1;
				if (c==0)
					{// end of cycle
					for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1]; // drop
					for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1]; // drop
					}
				else 	// cycle
					{
					for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1]; // drop
					stack[MAXSTACK-1]=c;
					if (l>=MAXLABEL) puts("LOOP MAXLABEL ERROR");
					else faylp=label[l];
					}
				}
			else if (!strcmp(buf,"=="))
				{
				stack[MAXSTACK-1]=(stack[MAXSTACK-2]==stack[MAXSTACK-1])?1:0;
				for (i=MAXSTACK-2;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"!="))
				{
				stack[MAXSTACK-1]=(stack[MAXSTACK-2]!=stack[MAXSTACK-1])?1:0;
				for (i=MAXSTACK-2;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,">="))
				{
				stack[MAXSTACK-1]=(stack[MAXSTACK-2]>=stack[MAXSTACK-1])?1:0;
				for (i=MAXSTACK-2;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"<="))
				{
				stack[MAXSTACK-1]=(stack[MAXSTACK-2]<=stack[MAXSTACK-1])?1:0;
				for (i=MAXSTACK-2;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"<"))
				{
				stack[MAXSTACK-1]=(stack[MAXSTACK-2]<stack[MAXSTACK-1])?1:0;
				for (i=MAXSTACK-2;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,">"))
				{
				stack[MAXSTACK-1]=(stack[MAXSTACK-2]>stack[MAXSTACK-1])?1:0;
				for (i=MAXSTACK-2;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"hex")) mode=HEX;
			else if (!strcmp(buf,"dec")) mode=DEC;
			else if (!strcmp(buf,"mode")) 
				{switch (mode)
					{
					case HEX: puts("mode hex"); break;
					case DEC: puts("mode dec"); break;
					default: puts("mode HELL");
					}
				}
			else if (!strcmp(buf,"quit")) return;
#include	"aux-skripts.c"
			else
				{puts0("\r\nUnknown operator: '");puts0(buf);puts("'");}
			}
		}
	if (/*(console==0)&&*/(fayl[faylp]==0)) break;
	}
if (console==1) goto l_begin;
puts("\r\nSkript finished");
}
