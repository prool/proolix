#define MAXLEN2	256

#define MAXSTACK	10
#define MAXLABEL	10
#define DEC	0
#define HEX	1

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

void skript(void)
{
char buf[MAXLEN2];
int i, line, file, rcode, console=0, mode=DEC;
int l, number;
char *cc;
char buf1[MAXLEN2];
int stack[MAXSTACK];
int label[MAXLABEL];

for (i=0;i<MAXSTACK;i++) stack[i]=0;
for (i=0;i<MAXLABEL;i++) label[i]=-1;

while(1)
	{
	puts0("Filename (? for dir, ! for console) > ");
	getsn(buf,MAXLEN2);
	if (buf[0]=='?') ls();
	else	break;
	}

if (buf[0]=='!') {console=1; puts("quit for quit");}
else if ((file=open(buf,0))==-1) {puts0("\r\nFile not found :("); return;}

puts("\r\nProol Skript Interpterer v.0\r\n");

// perviy prohod
if (console==0)
 {
 number=MAXLABEL;
 while(1)
	{
	readw(file,buf1,MAXLEN2);
	if (buf1[0])
		{
		//puts(buf1);
		if (isdigit(buf1[0])) number=atoi(buf1);
		else if (!strcmp(buf1,"label"))
			{
				if (number>=MAXLABEL) puts("MAXLABEL ERROR");
				else label[number]=lseek(file,0,SEEK_CUR);
			}
		}
	else break;
	}
 lseek(file,0,SEEK_SET);
 }

// vtoroy prohod

while(1)
	{
    	if (console==0) readw(file,buf,MAXLEN2); else {getsn(buf,MAXLEN2); puts("");}
	if (buf[0]==0) break;
	if (1)
		{// eval buf
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
			else if (!strcmp(buf,"inputstring")) getsn(buf1,MAXLEN2);
			else if (!strcmp(buf,"outputstring")) puts0(buf1);
			else if (!strcmp(buf,"outputint"))
				if (mode==DEC) putdec(atoi(buf1)); else puthex(atoi(buf1));
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
				{
				stack[MAXSTACK-1]=stack[MAXSTACK-2]/stack[MAXSTACK-1];
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
				if (mode==DEC) putdec(stack[MAXSTACK-1]); else puthex(stack[MAXSTACK-1]);
				if (console) puts("");
				for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,".b"))
				{
				puthex_b(stack[MAXSTACK-1]);
				if (console) puts("");
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
				else label[l]=lseek(file,0,SEEK_CUR);
				for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"goto"))
				{
				l=stack[MAXSTACK-1];
				if (l>=MAXLABEL) puts("GOTO MAXLABEL ERROR");
				else lseek(file,label[l],SEEK_SET);
				for (i=MAXSTACK-1;i>0;i--) stack[i]=stack[i-1];
				}
			else if (!strcmp(buf,"ifgoto"))
				{
				if (stack[MAXSTACK-1]!=0)
					{
					l=stack[MAXSTACK-2];
					if (l>=MAXLABEL) puts("IFGOTO MAXLABEL ERROR");
					else lseek(file,label[l],SEEK_SET);
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
					else lseek(file,label[l],SEEK_SET);
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
			else if (!strcmp(buf,"rewind")) lseek(file,0,SEEK_SET);
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
			else if (!strcmp(buf,"quit")) goto l_exit;
			else
				{puts0("\r\nUnknown operator: '");puts0(buf);puts("'");}
			}
		}
	}
l_exit:
if (console==0) close(file);
puts("\r\nSkript finished");
}
