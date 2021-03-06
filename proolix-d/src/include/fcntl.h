/*      fcntl.h

        Define flag values accessible to open.

        Copyright (c) Serge Pustovoitoff 1993-1997

        Portions Copyright (c) 1983, 1990 The Regents of the University of
        California. All rights reserved.

*/

#ifndef _FCNTL_H_
#define _FCNTL_H_

/*
       DOS Fn=3C Directory Attribute Flags

        񽼖񽔼񼭢񼆈� Directory Attribute Flags
         � � � � � � � 滥哪 1 = read only
         � � � � � � 滥哪� 1 = hidden
         � � � � � 滥哪哪 1 = system
         � � � � 滥哪哪� 1 = volume label  (exclusive)
         � � � 滥哪哪哪 1 = subdirectory
         � � 滥哪哪哪� 1 = archive
         滥聊哪哪哪哪 unused
*/

#define O_RDONLY             1
#define O_WRONLY             2
#define O_RDWR               4
#define O_CREAT         0x0100  /* create and open file */
#define O_TRUNC         0x0200  /* open with truncation */
#define O_EXCL          0x0400  /* exclusive open */
#define O_APPEND        0x0800  /* to end of file */

/* MSDOS special bits */
#define O_TEXT          0x4000  /* CR-LF translation    */
#define O_BINARY        0x8000  /* no translation       */

/* Proolix special flag */
#define O_FREOPEN       8

#endif /* !_FCNTL_H_ */
