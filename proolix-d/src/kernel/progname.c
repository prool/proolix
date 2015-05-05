/* Выделение только_имени программы (без расширения) из
полного пути (как в argv[0] */

#ifdef MSDOS
# define SLASH '\\'
# define FAR
#else
# define SLASH '/'
#  ifdef Proolix
#  define FAR far
#  define FAR
#  else
#  endif
#endif


char FAR *ProgName(char FAR *argv0)
{char FAR *cc, FAR *c1, FAR *path;
 char buff[80];

path=buff;
strcpy(path,argv0);

#ifdef MSDOS
if ((strlen(path))>2)
if (path[1]==':') path+=2;
#endif

if (path[0]==SLASH) path++;

cc=path;
while(*path++);
while(--path>cc) if (*path==SLASH) break;
if(path==cc) c1=cc; else c1=path+1;

#ifdef MSDOS
if ((cc=strchr(c1,'.'))!=NULL)*cc=0;
#endif
return c1;
}
