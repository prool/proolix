static void alloc(char * hexstr)
{void far * ptr; long m; unsigned Bolvan;
puts("");

m=htol(hexstr);

if (m==0)
  {printf("Usage: alloc <memory size in bytes>\n"); return; }

Bolvan=ProcessPar;
ProcessPar=0xFEDA;

printf("Allocated %8lX bytes\n",m);

ptr=malloc(m);
  if (ptr==NULL) puts("No memory");
  else {printf("%4X:%4X\n",FP_SEG(ptr),FP_OFF(ptr));}
ProcessPar=Bolvan;
}
/****************************************************************************/
static void kill_zombie(char * hexstr)
{void far * ptr; int m;
puts("");

m=htoi(hexstr);
printf("%4X ",m);
ptr=MK_FP(m,0);
free(ptr);
}
/****************************************************************************/
static void open_(char *s)
{int h;
printf ("h=%2i errno=%2i ", h=open(s,O_RDONLY), errno);
if (h!=-1) close(h);
}