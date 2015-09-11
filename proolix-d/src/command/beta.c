/* Proolix testing C-program */

#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <process.h>

#define PRINTF_DEBUG

#ifdef PRINTF_DEBUG
/* char far *far_global_init = "far_global_init"; */
char far *far_global_uninit;
char *near_global_init = "near_global_init";
char *near_global_uninit;
#endif

void main(void)
{
#ifdef PRINTF_DEBUG
char far *far_local_init = "far_local_init";
char far *far_local_uninit;
char *near_local_init = "near_local_init";
char *near_local_uninit;
#endif

#ifdef PRINTF_DEBUG
far_global_uninit="far_global_uninit";
far_local_uninit="far_local_uninit";
near_global_uninit="near_global_uninit";
near_local_uninit="near_local_uninit";

#if 1
printf("printf(literal)\n");
printf("local far init"); printf(far_local_init);
printf("local far uninit"); printf(far_local_uninit);
/* printf("global far init"); printf(far_global_init); */
printf("global far uninit"); printf(far_global_uninit);
#endif

printf("\nprintf():\n\n");
printf("local  far str initialized   %%Fs %Fs\n", far_local_init);
printf("local  far str uninitialized %%Fs %Fs\n", far_local_uninit);
printf("global far str uninitialized %%Fs %Fs\n", far_global_uninit);

printf("local  near str initialized   %%s   %s\n", near_local_init);
printf("local  near str uninitialized %%s   %s\n", near_local_uninit);
printf("global near str initialized   %%s   %s\n", near_global_init);
printf("global near str uninitialized %%s   %s\n", near_global_uninit);

printf("Printf 0 = %i\n",0);
exit(0);
printf("Printf 1 = %i\n",1);
#endif

}
