void hexdump(char *header, char huge *adr, int ctr)
{int i;
printf(header);
for (i=0;i<ctr;i++)printf("%02X",adr++);
}
