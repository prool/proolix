/* c-headers of system calls */

#define O_READ 0
#define O_WRITE 1
#define O_CREAT 2
#define O_APPEND 3

#define O_WRONLY 4
#define O_RDONLY 5
#define O_TRUNC 6

void syscall_test (void);
void stop(void);
unsigned char getchar(void);
void putch(unsigned char);
void puts0(char *s);
void puts(char *s);
char *getsn(char *str, int len);

unsigned short int peek(unsigned short int address);
unsigned char peek_far_b(unsigned short int segment, unsigned short int address);
void puthex1(char c);
void puthex_b(char c);
void puthex(int c);
void puthex_l(int c);

int open(char *path, int flags, ...);
int readc (int h, char *c);
int writec(int h, char c);
int close(int h);

int strcmp (const char  *s1, const char  *s2);
