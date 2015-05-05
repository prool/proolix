void process_partition_table (char far *MBRBuf)
{
struct MBRstru far *MBR;
char ExtBuf [SECTOR_SIZE];
int i;

MBR=(void far *)MBRBuf;
for (i=0; i<4; i++)
  {
  if (MBR->Partition[i].system_indicator)
    {
    if (NextDrive<LASTDRIVE)
      {
      Devices[NextDrive].head=MBR->Partition[i].begin_head;
      Devices[NextDrive].sec=MBR->Partition[i].begin_sec;
      Devices[NextDrive].trk=MBR->Partition[i].begin_cyl;
      Devices[NextDrive].system_indicator=MBR->Partition[i].system_indicator;
      switch(MBR->Partition[i].system_indicator)
        {
        case 1 : Devices[NextDrive].FileSystem=FAT12; break;
        case 4 :
        case 6 : Devices[NextDrive].FileSystem=FAT16; break;
        case 0x0C: Devices[NextDrive].FileSystem=FAT32LBA; break;
        default: Devices[NextDrive].FileSystem=NO_FAT;
        }
      switch(MBR->Partition[i].system_indicator)
        {
        case EXTEND:
        case EXTEND2: Devices[NextDrive].dos_disk=' '; break;
        default: Devices[NextDrive].dos_disk=NextDOSDrive++;
        }
      Devices[NextDrive].ResSecs=MBR->Partition[i].preceding_sec + 1;
      Devices[NextDrive].MaxSectors=MBR->Partition[i].total_sec
      +Devices[NextDrive].ResSecs;
      NextDrive++;
      #if 1
      if((MBR->Partition[i].system_indicator==EXTEND)||(MBR->Partition[i].system_indicator==EXTEND2))
        { /*     extended partition */
        if (ReadPhysSec2 (_HDD_,MBR->Partition[i].begin_sec,
        MBR->Partition[i].begin_head,MBR->Partition[i].begin_cyl,ExtBuf))
          printf ("boot: extend boot sector read error\n");
        process_partition_table(ExtBuf);
        }
      #endif
      }
    }
  }
}