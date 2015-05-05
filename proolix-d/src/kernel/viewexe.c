#include <stdio.h>

#include "..\include\limits.h"
#include "..\include\struct.h"

void ViewEXE (struct exe_header far *Buf)
{long i, j;
printf("Bytes on last page %04X\n",Buf->PartPag );
printf("Pages in file %04X\n",Buf->PageCnt );
printf("Relocations %04X\n",Buf->ReloCnt );
printf("Paragraphe in header %04X\n",Buf->HdrSize );
printf("MinMem %04X par\n",Buf->MinMem  );
printf("MaxMem %04X par\n",Buf->MaxMem  );
printf("SS:SP %04X:%04X\n",Buf->ReloSS,Buf->ExeSP);
printf("CheckSum %04X\n",Buf->ChkSum  );
printf("CS:IP %04X:%04X\n",Buf->ReloCS,Buf->ExeIP);
printf("Relocation table addr %04X\n",Buf->TablOff );
/* printf("Overlay No. %1X\n",Buf->Overlay); */
}
/* �� [TECH Help!]

����������������������������������������������������������������������������ͻ
�                        ������� ��������� 䠩�� EXE                       �
����������������������������������������������������������������������������ͼ

����.  ����� ����ন���
������ ����� �����������������������������������������������������������������
            �������Ŀ
 +0      2  �4Dh 5aH� "�������" 䠩�� .EXE ('MZ')
            �������Ĵ
 +2      2  �PartPag� ����� �������� ��᫥���� ��࠭��� (���筮 ����������)
            �������Ĵ
 +4      2  �PageCnt� ����� ��ࠧ� � 512-���⮢�� ��࠭���, ������ ���������
            �������Ĵ
 +6      2  �ReloCnt� �᫮ ������⮢ � ⠡��� ��६�饭��
            �������Ĵ
 +8      2  �HdrSize� ����� ��������� � 16-���⮢�� ��ࠣ���
            �������Ĵ
+0aH     2  �MinMem � ������ �ॡ㥬�� ����� �� ���殬 �ணࠬ�� (��ࠣ���)
            �������Ĵ
+0cH     2  �MaxMem � ���ᨬ� �ॡ㥬�� ����� �� ���殬 �ணࠬ�� (��ࠣ���)
            �������Ĵ
+0eH     2  �ReloSS � ᥣ���⭮� ᬥ饭�� ᥣ���� �⥪� (��� ��⠭���� SS)
            �������Ĵ
+10H     2  �ExeSP  � ���祭�� ॣ���� SP (㪠��⥫� �⥪�) �� ����᪥
            �������Ĵ
+12H     2  �ChkSum � ����஫쭠� �㬬� (����⥫쭠� �㬬� ��� ᫮� � 䠩��)
            �������Ĵ
+14H     2  �ExeIP  � ���祭�� ॣ���� IP (㪠��⥫� ������) �� ����᪥
            �������Ĵ
+16H     2  �ReloCS � ᥣ���⭮� ᬥ饭�� �������� ᥣ���� (��� ��⠭���� CS)
            �������Ĵ
+18H     2  �TablOff� ᬥ饭�� � 䠩�� 1-�� ������� ��६�饭�� (��� 001cH)
            �������Ĵ
+1aH     2  �Overlay� ����� ���૥� (0 ��� �������� �����)
            ���������
1cH         ࠧ��� �ଠ�஢����� ���樨 ��������� EXE
            ����������������� � � ���������������Ŀ ������ ��६�饭��. ��砫�
+ ?     4*? � ᬥ�.  ᥣ����  � ᬥ�.  ᥣ���� �� ᬥ饭�� [EXE+18H]. �����
            ����������������� � � ����������������� [EXE+6] 4-���⮢�� �������.
+ ?     ?   �ய�� �� �࠭��� ��ࠣ��
+ ?     ?   ��砫� ��ࠧ� �ணࠬ��
��������������������������������������������������������������������������������
��᪮��� EXE-䠩� ����� ���� ����㦥� � �� ᥣ����, �� ��᮫��� ᥣ����-
�� ��뫪� (FAR CALL, ������� 㪠��⥫�, ��뫪� ⨯� MOV AX,data_seg) ������
���� �ਢ����� � ���ᠬ �����, ᮮ⢥�����騬 ����㧪�. ���� �ਢ����� 蠣�,
�ᯮ��㥬� �ணࠬ��� ����㧪� DOS (�㭪�� 4bH ) �� ����㧪� 䠩�� EXE:

1.  ᮧ���� PSP ���।�⢮� �㭪樨 DOS 26H

2.  ������ 1cH ���� 䠩�� EXE (�ଠ�஢����� ����� ��������� EXE)
    � �������� ������� �����

3.  ��।����� ࠧ��� ����� = ( (PageCnt*512)-(HdrSize*16) ) - PartPag

4.  ��।����� 䠩����� ᬥ饭�� ����㦠����� ����� = (HdrSize * 16)

5.  ����� ᥣ����� ����, START_SEG, ��� ����㧪� (���筮  PSP + 10H)

6.  ����� ����� � ������� �����, ��稭������� � ���� START_SEG:0000

7.  LSEEK (���. 㪠��⥫� 䠩��) �� ��砫� ⠡���� ��६�饭�� (TablOff)

8.  ��� ������� ������� ��६�饭�� (ReloCnt):
    a. ����� ������� ��� ��� 16-��⮢�� ᫮�� (I_OFF,I_SEG)
    b. ���᫨�� RELO_SEG=(START_SEG+I_SEG) (���� ��६�頥��� ��뫪�)
    c. ������� ᫮�� �� ����� RELO_SEG:I_OFF (������ ⥪�饥 ���祭��)
    d. �ਡ����� START_SEG � �⮬� ᫮�� (�믮����� �ਢ離� ᥣ����)
    e. �������� १���� ���⭮ �� ��� ����� (RELO_SEG:I_OFF)

9.  ���।����� ������ ��� �ணࠬ�� ᮣ��᭮ MaxMem � MinMem

10. ���樠����஢��� ॣ����� � �������� �ணࠬ��:
    a. ES = DS = PSP
    b. AX = ��ࠦ��� ���४⭮��� �����䨪��஢ ��᪮� � ��������� ��ப�
    c. SS = START_SEG+ReloSS, SP = ExeSP
    d. CS = START_SEG+ReloCS, IP = ExeIP
    (�������: PUSH seg; PUSH offset; RETF)
 */
