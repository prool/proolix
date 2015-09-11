/*      process.h

        Symbols and structures for process management.

        Copyright (c) Serge Pustovoitoff 1994-1999

*/

#include <sys/types.h>

int     exec(char far *path);
int     execl(char far *path, char far *arg0, ...);
int     execle(char far *path, char far *arg0, ...);
int     execlp(char far *path, char far *arg0, ...);
int     execlpe(char far *path, char far *arg0, ...);
int     execv(char far *path, char far *argv[]);
int     execve(char far*name, char  far*_argv[], char far*far*env);
int     execvp(char far *path, char far *argv[]);
int     execvpe(char far *path, char far *argv[], char far *far *env);
void    exit(int status);
void    _exit(int status);
int     system(const char far *command); /* Proolix */
int     fork (void); /* Proolix */
int     service(int fn); /* Proolix: kernel service: fn=0 - ps */
void    ps_(void); /* Proolix */
int     wait (pid_t pid); /* Proolix */
int     sema_lock (int far *sema); /* Proolix */
void    sema_unlock (int far *sema); /* Proolix */
