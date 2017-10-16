/* c-headers of system calls */

void syscall_test (void);
void stop(void);
unsigned char getchar(void);
void putch(unsigned char);
void puts0(char *s);
void puts(char *s);
unsigned short int peek(unsigned short int address);
unsigned char peek_far_b(unsigned short int segment, unsigned short int address);
void puthex1(char c);
void puthex_b(char c);
void puthex(int c);
void puthex_l(int c);
