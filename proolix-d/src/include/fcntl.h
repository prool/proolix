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

        ³7³6³5³4³3³2³1³0³ Directory Attribute Flags
         ³ ³ ³ ³ ³ ³ ³ ÀÄÄÄ 1 = read only
         ³ ³ ³ ³ ³ ³ ÀÄÄÄÄ 1 = hidden
         ³ ³ ³ ³ ³ ÀÄÄÄÄÄ 1 = system
         ³ ³ ³ ³ ÀÄÄÄÄÄÄ 1 = volume label  (exclusive)
         ³ ³ ³ ÀÄÄÄÄÄÄÄ 1 = subdirectory
         ³ ³ ÀÄÄÄÄÄÄÄÄ 1 = archive
         ÀÄÁÄÄÄÄÄÄÄÄÄ unused
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
