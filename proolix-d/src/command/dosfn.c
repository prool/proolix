#include <stdio.h>

void dos_fn_man (char fn)
{
printf("\n");
switch (fn)
{
case 0:   printf("Program terminate"); break;
case 1:   printf("Keyboard input with echo"); break;
case 2:   printf("Display output"); break;
case 3:   printf("Wait for auxiliary device input"); break;
case 4:   printf("Auxiliary output"); break;
case 5:   printf("Printer output"); break;
case 6:   printf("Direct console I/O"); break;
case 7:   printf("Wait for direct console input without echo"); break;
case 8:   printf("Wait for console input without echo"); break;
case 9:   printf("Print string"); break;
case 0xA: printf("Buffered keyboard input"); break;
case 0xB: printf("Check standard input status"); break;
case 0xC: printf("Clear keyboard buffer, invoke keyboard function"); break;
case 0xD: printf("Disk reset"); break;
case 0xE: printf("Select disk"); break;
case 0xF: printf("Open file using FCB"); break;
case 0x10:printf("Close file using FCB"); break;
case 0x11:printf("Search for first entry using FCB"); break;
case 0x12:printf("Search for next entry using FCB"); break;
case 0x13:printf("Delete file using FCB"); break;
case 0x14:printf("Sequential read using FCB"); break;
case 0x15:printf("Sequential write using FCB"); break;
case 0x16:printf("Create a file using FCB"); break;
case 0x17:printf("Rename file using FCB"); break;
case 0x18:printf("DOS dummy function (CP/M) (not used/listed)"); break;
case 0x19:printf("Get current default drive"); break;
case 0x1A:printf("Set disk transfer address"); break;
case 0x1B:printf("Get allocation table information"); break;
case 0x1C:printf("Get allocation table info for specific device"); break;
case 0x1D:printf("DOS dummy function (CP/M) (not used/listed)"); break;
case 0x1E:printf("DOS dummy function (CP/M) (not used/listed)"); break;
case 0x1F:printf("Get pointer to default drive parameter table (undocumented)"); break;
case 0x20:printf("DOS dummy function (CP/M) (not used/listed)"); break;
case 0x21:printf("Random read using FCB"); break;
case 0x22:printf("Random write using FCB"); break;
case 0x23:printf("Get file size using FCB"); break;
case 0x24:printf("Set relative record field for FCB"); break;
case 0x25:printf("Set interrupt vector"); break;
case 0x26:printf("Create new program segment"); break;
case 0x27:printf("Random block read using FCB"); break;
case 0x28:printf("Random block write using FCB"); break;
case 0x29:printf("Parse filename for FCB"); break;
case 0x2A:printf("Get date"); break;
case 0x2B:printf("Set date"); break;
case 0x2C:printf("Get time"); break;
case 0x2D:printf("Set time"); break;
case 0x2E:printf("Set/reset verify switch"); break;
case 0x2F:printf("Get disk transfer address"); break;
case 0x30:printf("Get DOS version number"); break;
case 0x31:printf("Terminate and stay resident"); break;
case 0x32:printf("Get pointer to drive parameter table (undocumented)"); break;
case 0x33:printf("Get/set Ctrl-Break check state & get boot drive"); break;
case 0x34:printf("Get address to DOS critical flag (undocumented)"); break;
case 0x35:printf("Get interrupt vector"); break;
case 0x36:printf("Get disk free space"); break;
case 0x37:printf("Get/set switch character (undocumented)"); break;
case 0x38:printf("Get/set country dependent information"); break;
case 0x39:printf("Create subdirectory (mkdir)"); break;
case 0x3A:printf("Remove subdirectory (rmdir)"); break;
case 0x3B:printf("Change current subdirectory (chdir)"); break;
case 0x3C:printf("Create file using handle"); break;
case 0x3D:printf("Open file using handle"); break;
case 0x3E:printf("Close file using handle"); break;
case 0x3F:printf("Read file or device using handle"); break;
case 0x40:printf("Write file or device using handle"); break;
case 0x41:printf("Delete file"); break;
case 0x42:printf("Move file pointer using handle"); break;
case 0x43:printf("Change file mode"); break;
case 0x44:printf("I/O control for devices (IOCTL)"); break;
case 0x45:printf("Duplicate file handle"); break;
case 0x46:printf("Force duplicate file handle"); break;
case 0x47:printf("Get current directory"); break;
case 0x48:printf("Allocate memory blocks"); break;
case 0x49:printf("Free allocated memory blocks"); break;
case 0x4A:printf("Modify allocated memory blocks"); break;
case 0x4B:printf("EXEC load and execute program (func 1 undocumented)"); break;
case 0x4C:printf("Terminate process with return code"); break;
case 0x4D:printf("Get return code of a sub-process"); break;
case 0x4E:printf("Find first matching file"); break;
case 0x4F:printf("Find next matching file"); break;
case 0x50:printf("Set current process id (undocumented)"); break;
case 0x51:printf("Get current process id (undocumented)"); break;
case 0x52:printf("Get pointer to DOS 'INVARS' (undocumented)"); break;
case 0x53:printf("Generate drive parameter table (undocumented)"); break;
case 0x54:printf("Get verify setting"); break;
case 0x55:printf("Create PSP (undocumented)"); break;
case 0x56:printf("Rename file"); break;
case 0x57:printf("Get/set file date and time using handle"); break;
case 0x58:printf("Get/set memory allocation strategy (3.x+, undocumented)"); break;
case 0x59:printf("Get extended error information (3.x+)"); break;
case 0x5A:printf("Create temporary file (3.x+)"); break;
case 0x5B:printf("Create new file (3.x+)"); break;
case 0x5C:printf("Lock/unlock file access (3.x+)"); break;
case 0x5D:printf("Critical error information (undocumented 3.x+)"); break;
case 0x5E:printf("Network services (3.1+)"); break;
case 0x5F:printf("Network redirection (3.1+)"); break;
case 0x60:printf("Get fully qualified file name (undocumented 3.x+)"); break;
case 0x62:printf("Get address of program segment prefix (3.x+)"); break;
case 0x63:printf("Get system lead byte table (MSDOS 2.25 only)"); break;
case 0x64:printf("Set device driver look ahead  (undocumented 3.3+)"); break;
case 0x65:printf("Get extended country information (3.3+)"); break;
case 0x66:printf("Get/set global code page (3.3+)"); break;
case 0x67:printf("Set handle count (3.3+)"); break;
case 0x68:printf("Flush buffer (3.3+)"); break;
case 0x69:printf("Get/set disk serial number (undocumented DOS 4.0+)"); break;
case 0x6A:printf("DOS reserved (DOS 4.0+)"); break;
case 0x6B:printf("DOS reserved"); break;
case 0x6C:printf("Extended open/create (4.x+)"); break;
case 0xF8:
case 0xF9:
case 0xFA:
case 0xFB:
case 0xFC:
case 0xFD:
case 0xFE:
case 0xFF:
          printf("OEM INT 21 handler (undocumented)"); break;
default  :printf("unknown");
}
}
