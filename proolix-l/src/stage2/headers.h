// C-headers for c utilities

#define NULL 0
typedef unsigned int size_t;

void stop(void);
void putch(char c);
void puts0(char *s);
int getch(void);
size_t strlen (const char *s);
char *strchr (const char *str, int c);
char  * strncpy (char  * dest, const char  * src, size_t maxlen);
