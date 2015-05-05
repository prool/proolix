/*      conio.h

        Direct MSDOS console input/output.

        Copyright (c) Serge Pustovoitoff 1993-1997

*/
#if     !defined(__VIDEO)
#define __VIDEO

#define setcolor(c) textattr ((c))

#ifndef __OLDCONIO__

#define BLINK           128     /* blink bit */
#define Norm              2     /* Prool */
#define Inverse        0x70     /* Prool */

int       getcolor        (void);

void      clrscr          (void);
void      gotoxy          (int x, int y);
void      textattr        (int newattr);
void      textbackground  (int newcolor);
void      textcolor       (int newcolor);
void      textmode        (int newmode);
int       wherex          (void);
int       wherey          (void);
#endif

int       getch           (void);
int       getch0          (void);
int       getche          (void);
char    * getpass         (const char *prompt);
int       kbhit           (void);
int       kbhit0          (void);
int       putch           (int c);
int       ungetch         (int ch);

#endif
