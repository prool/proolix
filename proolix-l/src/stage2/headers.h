// C-headers for c utilities

#define NULL 0
typedef unsigned int size_t;

void stop(void);
void putch(char c);
void putch2(char c);
void putch3(char c);
void puts0(char *s);
void putch_color(char c, char attrib);
int getch(void);
size_t strlen (const char *s);
char *strchr (const char *str, int c);
char  * strncpy (char  * dest, const char  * src, size_t maxlen);

char get_row(void);
char get_col(void);
void setpos (char row, char col);

char get_color(void);
void set_color(char color);

void help(void);
