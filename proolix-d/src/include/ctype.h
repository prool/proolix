/*      ctype.h

        Defines the ctype macros.

        Copyright (c) Serge Pustovoitoff 1993-1997

*/
#define isascii(c)      ((unsigned)(c) < 128)

#define _toupper(c)     ((c) + 'A' - 'a')
#define _tolower(c)     ((c) + 'a' - 'A')
#define toascii(c)      ((c) & 0x7f)

int      tolower(int ch);
int      toupper(int ch);

int      isgraph(int ch);
int      isprint(int ch);

int isalnum (int);
int isalpha (int);
int iscntrl (int);
int isdigit (int);
int islower (int);
int ispunct (int);
int isspace (int);
int isupper (int);
int isxdigit(int);
