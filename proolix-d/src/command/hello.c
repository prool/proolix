#include <stdio.h>
#include <io.h>
#include <dir.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>

void main(void)
{
char wd [NAME_MAX+3];
printf("%Fs\n",getcwd(wd,NAME_MAX+3));
}
