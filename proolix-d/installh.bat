@echo off
cls
echo 旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
echo � 댂飡젷ワ與� 럱 Proolix 췅 a: � c: (boot manager 췅 a:, 甸昔 췅 c: )�
echo �                                                                    �
echo � Installator (install.bat) version H-0.0.2.1 30-Sep-98              �
echo �                                                                    �
echo � 뒥�젷��, � ぎ獸昔� �졹캙ぎ쥯� �젵� slime?.arj                      �
echo � ㄾウ�� 〓筍 收ゃ蟯�.                                               �
echo � � 信循�⒰手� a: ㄾウ췅 〓筍 葉飡좑 ㄸ稅β�!                        �
echo �                                                                    �
echo � 볡殊收, ▲�-醒も�贍 ㄸ稅� � ㄸ稅β�, � �젶┘ ㏇�                   �
echo � Þ兒席졿⑨, 貰ㄵ逝좈좑碎 췅 ㄸ稅� � ㄸ稅β�, Мｃ� 〓筍 쭬��暹��� !�
echo � 뜝━ⓥ� CTRL BREAK, αエ 귣 ��誓ㅳ쵟エ.                            �
echo � � �昔殊˛�� 笹晨젰 췅━ⓥ� ANY KEY                                 �
echo 읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸
pause>nul
cls
src\boot\bp -d0 -i -y src\boot\boots.bin
if errorlevel 1 goto Error
echo Boot manager to diskette a:
copy src\bm\booth a:\boot > nul
echo Kernel to disk c:
copy src\kernel\kernel c:\kernel > nul
echo on
REM
REM Doc to disk c:
copy *.doc c:\
REM
REM Files description to disk c:
copy files.bbs c:\
REM
REM Utilities to disk c:
mkdir c:\bin
copy src\command\kernel.ovl c:\ > nul
REM a:
REM cd \bin
REM c:
copy src\command\*. c:\bin
REM Fonts to disk c:
copy src\command\*.fnt c:\bin
REM
REM Man
mkdir c:\usr
mkdir c:\usr\man
copy man\*.* c:\usr\man
REM
REM Installation done.
REM Reboot, please, from diskette a:
REM Press, please, Ctrl Alt Del
REM
goto End
:Error
echo !
echo ! Installation error
echo !
:End
