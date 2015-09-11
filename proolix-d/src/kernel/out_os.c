#include <stdio.h>

void out_os (unsigned char c)
{
switch(c)
  { /*          Values for operating system indicator:                 */

  case 0x00: printf("none   "); break; /* 00h    empty */
  case 0x01: printf("FAT-12 "); break; /* 01h    DOS 12-bit FAT */
  #if 0
  case 0x02: printf("XENIX /"); break; /* 02h    XENIX root file system */
  case 0x03: printf("XENIX u"); break; /* 03h    XENIX /usr file system (obsolete) */
  #endif
  case 0x04: printf("FAT-16 "); break; /* 04h    DOS 16-bit FAT < 32M */
  case 0x05: printf("EXTEND "); break; /* 05h    DOS 3.3+ extended partition */
  case 0x06: printf("LARGE  "); break; /* 06h    DOS 3.31+ Large File System (16-bit FAT >= 32M), "Big DOS" */
  case 0x07: printf("HPFS/NT"); break; /* 07h    QNX */
                                       /* 07h    OS/2 HPFS */
                                       /* 07h    Windows NT NTFS */
                                       /* 07h    Advanced Unix */
  #if 0
  case 0x08: printf("OS2 AIX"); break; /* 08h    OS/2 (v1.0-1.3 only) */
                                       /* 08h    AIX bootable partition, SplitDrive */
                                       /* 08h    Commodore DOS */
                                       /* 08h    DELL partition spanning multiple drives */
  case 0x09: printf("AIX    "); break; /* 09h    AIX data partition */
                                       /* 09h    Coherent filesystem */
  case 0x0A: printf("OS2 Coh"); break; /* 0Ah    OS/2 Boot Manager */
                                       /* 0Ah    OPUS */
                                       /* 0Ah    Coherent swap partition */
  #endif
  case 0x0B: printf("FAT-32 "); break; /* 0Bh    Windows95 with 32-bit FAT */
  case 0x0C: printf("FAT32LB"); break; /* 0Ch    Windows95 with 32-bit FAT (using LBA-mode INT 13 extensions) */
  case 0x0E: printf("VFATLBA"); break; /* 0Eh    logical-block-addressable VFAT (same as 06h but using LBA-mode INT 13) "FAT-16 LBA" */
  case 0x0F: printf("EXTENDL"); break; /* 0Fh    logical-block-addressable VFAT (same as 05h but using LBA-mode INT 13) "FAT-32 extended LBA" */
  #if 0
  case 0x10: printf("OPUS   "); break; /* 10h    OPUS */
  case 0x11: printf("OS/2   "); break; /* 11h    OS/2 Boot Manager, hidden DOS 12-bit FAT */
  case 0x12: printf("Compaq "); break; /* 12h    Compaq Diagnostics (config) partition */
  case 0x14: printf("OS/2   "); break; /* 14h    (resulted from using Novell DOS 7.0 FDISK to delete Linux Native part) */
                                       /* 14h    OS/2 Boot Manager hidden sub-32M 16-bit FAT partition */
  case 0x16: printf("OS/2   "); break; /* 16h    OS/2 Boot Manager hidden over-32M 16-bit FAT partition */
  case 0x17: printf("OS/2   "); break; /* 17h    OS/2 Boot Manager hidden HPFS partition */
  case 0x18: printf("WinSwap"); break; /* 18h    AST special Windows swap file ("Zero-Volt Suspend" partition) */
  case 0x19: printf("Photon "); break; /* 19h    Willowtech Photon coS */
  case 0x1E: printf("Hiden95"); break; /* 1Eh    Hidden FAT95 */
  case 0x20: printf("Willow "); break; /* 20h    Willowsoft Overture File System (OFS1) */
  case 0x21: printf("reserv "); break; /* 21h    officially listed as reserved */
  case 0x23: printf("reserv "); break; /* 23h    officially listed as reserved */
  case 0x24: printf("NEC    "); break; /* 24h    NEC MS-DOS 3.x */
  case 0x26: printf("reserv "); break; /* 26h    officially listed as reserved */
  case 0x31: printf("reserv "); break; /* 31h    officially listed as reserved */
  case 0x33: printf("reserv "); break; /* 33h    officially listed as reserved */
  case 0x34: printf("reserv "); break; /* 34h    officially listed as reserved */
  case 0x36: printf("reserv "); break; /* 36h    officially listed as reserved */
  case 0x38: printf("Theos  "); break; /* 38h    Theos */
  case 0x3C: printf("PQMagic"); break; /* 3Ch    PowerQuest PartitionMagic recovery partition */
  case 0x40: printf("VENIX  "); break; /* 40h    VENIX 80286 */
  case 0x41: printf("Linux  "); break; /* 41h    Linux, Personal RISC Boot */
  case 0x42: printf("LinuxSw"); break; /* 42h    SFS (Secure File System) by Peter Gutmann */
                                       /*        Linux swap (sharing disk with DRDOS) */
  case 0x43: printf("Linux  "); break; /* 43h    Linux native (sharing disk with DRDOS) */
  case 0x50: printf("OnTrack"); break; /* 50h    OnTrack Disk Manager, read-only partition */
  case 0x51: printf("NOVEL  "); break; /* 51h    OnTrack Disk Manager, read/write partition */
                                       /* 51h    NOVEL */
  case 0x52: printf("CP/M   "); break; /* 52h    CP/M */
                                       /* 52h    Microport System V/386 */
  case 0x53: printf("OnTrack"); break; /* 53h    OnTrack Disk Manager */
  case 0x54: printf("OnTrack"); break; /* 54h    OnTrack Disk Manager (Dynamic Drive Overlay) */
  case 0x55: printf("EZ     "); break; /* 55h    EZ-Drive */
  case 0x56: printf("GoldenB"); break; /* 56h    GoldenBow VFeature */
  case 0x5C: printf("Priam  "); break; /* 5Ch    Priam EDisk */
  case 0x61: printf("SpeedSt"); break; /* 61h    SpeedStor */
  case 0x63: printf("SysV386"); break; /* 63h    Unix SysV/386, 386/ix (SCO, ISC UNIX, UnixWare, ...) */
                                       /* 63h    Mach, MtXinu BSD 4.3 on Mach */
                                       /* 63h    GNU HURD */
  case 0x64: printf("Novell "); break; /* 64h    Novell NetWare 286 */
  case 0x65: printf("Novell "); break; /* 65h    Novell NetWare (3.11) */
  case 0x67: printf("Novell "); break; /* 67h    Novell */
  case 0x68: printf("Novell "); break; /* 68h    Novell */
  case 0x69: printf("Novell "); break; /* 69h    Novell */
  case 0x70: printf("DiskSec"); break; /* 70h    DiskSecure Multi-Boot */
  case 0x71: printf("reserv "); break; /* 71h    officially listed as reserved */
  case 0x73: printf("reserv "); break; /* 73h    officially listed as reserved */
  case 0x74: printf("reserv "); break; /* 74h    officially listed as reserved */
  case 0x75: printf("PC/IX  "); break; /* 75h    PC/IX */
  case 0x76: printf("reserv "); break; /* 76h    officially listed as reserved */
  case 0x77: printf("QNX    "); break; /* 77h    QNX4.x */
  case 0x78: printf("QNX    "); break; /* 78h    QNX4.x 2nd part */
  case 0x79: printf("QNX    "); break; /* 79h    QNX4.x 3rd part */
  #endif
  case 0x80: printf("Minix  "); break; /* 80h    Minix v1.1 - 1.4a */
  case 0x81: printf("Linux  "); break; /* 81h    Minix v1.4b+ */
                                       /* 81h    Linux */
                                       /* 81h    Mitac Advanced Disk Manager */
  case 0x82: printf("Linux  "); break; /* 82h    Linux Swap partition */
                                       /* 82h    Prime */
  case 0x83: printf("Linux  "); break; /* 83h    Linux native file system (ext2fs/xiafs) */
  #if 0
  case 0x84: printf("OS/2   "); break; /* 84h    OS/2-renumbered type 04h partition (related to hiding DOS C: drive) */
  #endif
  case 0x85: printf("LinuxEx"); break; /* 85h    Linux extended partition */
  case 0x86: printf("WinNT  "); break; /* 86h    NTFS, FAT16 volume/stripe set (Windows NT) */
  case 0x87: printf("HPFS/NT"); break; /* 87h    HPFS Fault-Tolerant mirrored partition */
                                       /* 87h    NTFS volume/stripe set */
  #if 0
  case 0x93: printf("Amoeba "); break; /* 93h    Amoeba file system */
  case 0x94: printf("Amoeba "); break; /* 94h    Amoeba bad block table */
  case 0xA0: printf("Phoenix"); break; /* A0h    Phoenix NoteBIOS Power Management "Save-to-Disk" partition */
                                       /*        IBM Thinkpad hibernation partition */
  case 0xA1: printf("reserv "); break; /* A1h    officially listed as reserved */
  case 0xA3: printf("reserv "); break; /* A3h    officially listed as reserved */
  case 0xA4: printf("reserv "); break; /* A4h    officially listed as reserved */
  case 0xA5: printf("386BSD "); break; /* A5h    Net,Free,Open,386BSD and BSD/OS */
  case 0xA6: printf("reserv "); break; /* A6h    officially listed as reserved */
  case 0xA7: printf("NEXT   "); break; /* A7h    NEXTSTEP */
  case 0xB1: printf("reserv "); break; /* B1h    officially listed as reserved */
  case 0xB3: printf("reserv "); break; /* B3h    officially listed as reserved */
  case 0xB4: printf("reserv "); break; /* B4h    officially listed as reserved */
  case 0xB6: printf("reserv "); break; /* B6h    officially listed as reserved */
  case 0xB7: printf("BSDI   "); break; /* B7h    BSDI file system (secondarily swap) */
  case 0xB8: printf("BSDI   "); break; /* B8h    BSDI swap partition (secondarily file system) */
  case 0xC0: printf("CTOS   "); break; /* C0h    CTOS */
  case 0xC1: printf("DR DOS "); break; /* C1h    DR DOS 6.0 LOGIN.EXE-secured 12-bit FAT partition */
  case 0xC4: printf("DR DOS "); break; /* C4h    DR DOS 6.0 LOGIN.EXE-secured 16-bit FAT partition < 32M */
  case 0xC6: printf("DR DOS "); break; /* C6h    DR DOS 6.0 LOGIN.EXE-secured Huge partition >= 32M */
                                       /* C6h    corrupted FAT16 volume/stripe set (Windows NT) */
  case 0xC7: printf("Syrinx "); break; /* C7h    Syrinx Boot */
                                       /* C7h    corrupted NTFS volume/stripe set */
  case 0xD8: printf("CP/M-86"); break; /* D8h    CP/M-86 */
  case 0xDB: printf("CP/M   "); break; /* DBh    CP/M, Concurrent CP/M, Concurrent DOS */
                                       /* DBh    CTOS (Convergent Technologies OS -Unisys) */
  case 0xE1: printf("SpeedSt"); break; /* E1h    DOS access or SpeedStor 12-bit FAT extended partition */
  case 0xE3: printf("DOS r/o"); break; /* E3h    DOS read-only */
                                       /* E3h    SpeedStor, Storage Dimensions */
  case 0xE4: printf("SpeedSt"); break; /* E4h    SpeedStor 16-bit FAT extended partition < 1024 cyl */
  case 0xE5: printf("reserv "); break; /* E5h    officially listed as reserved */
  case 0xE6: printf("reserv "); break; /* E6h    officially listed as reserved */
  case 0xF1: printf("Storage"); break; /* F1h    SpeedStor, Storage Dimensions */
  #endif
  case 0xF2: printf("DOS sec"); break; /* F2h    DOS 3.3+ secondary partition */
  #if 0
  case 0xF3: printf("reserv "); break; /* F3h    officially listed as reserved */
  case 0xF4: printf("SpeedSt"); break; /* F4h    SpeedStor large partition, Storage Dimensions */
  case 0xF6: printf("reserv "); break; /* F6h    officially listed as reserved */
  case 0xFE: printf("LANstep"); break; /* FEh    LANstep */
                                       /* FEh    IBM PS/2 IML */
                                       /*        SpeedStor > 1024 cyl. */
  case 0xFF: printf("XENIX  "); break; /* FFh    Xenix bad block table */
  #endif
  default  : printf("unkn %02X",c);
  }
}