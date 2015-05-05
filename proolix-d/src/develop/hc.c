/* memory model compact */

#include <stdio.h>
#include <io.h>
#include <conio.h>
#include <process.h>

char s1[]=" static string ";

void main(void)
{
char s2[]=" dynamic string ";
printf("Hello,\n");
printf(s1);
printf(s2);
printf("\npress any key\n");
getch();
exit(0);
}
