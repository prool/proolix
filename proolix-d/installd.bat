@echo off
cls
echo ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
echo ³ ˆ­áâ ««ïæ¨ï Ž‘ Proolix ­  à §¤¥« d: FAT-12                         ³
echo ³                                                                    ³
echo ³ Installator (install.bat) version D-0.0.3.1  24-Feb-98             ³
echo ³                                                                    ³
echo ³ Š â «®£, ¢ ª®â®à®¬ à á¯ ª®¢ ­ ä ©« slime?.arj                      ³
echo ³ ¤®«¦¥­ ¡ëâì â¥ªãé¨¬.                                               ³
echo ³                                                                    ³
echo ³ “çâ¨â¥, ¡ãâ-á¥ªâ®à ¤¨áª  ¨ Master Boot Record,   â ª¦¥ ¢áï         ³
echo ³ ¨­ä®à¬ æ¨ï, á®¤¥à¦ é ïáï ­  ¤¨áª¥, ¬®£ãâ ¡ëâì § ¯®àç¥­ë !          ³
echo ³  ¦¬¨â¥ CTRL BREAK, ¥á«¨ ‚ë ¯¥à¥¤ã¬ «¨.                            ³
echo ³ ‚ ¯à®â¨¢­®¬ á«ãç ¥ ­ ¦¬¨â¥ ANY KEY                                 ³
echo ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
pause>nul
cls
src\boot\bp -d@ boot.mbr
src\boot\bp -d3 -i -a0 boot.mbr
src\boot\bp -d4 -i src\boot\booth12.bin
copy src\bm\boot d:\boot > nul
echo Kernel to disk d:
copy src\kernel\kernel d:\kernel > nul
echo on
REM
REM Doc to disk d:
copy *.doc d:\
REM
REM Files description to disk d:
copy files.bbs d:\
REM
REM Utilities to disk d:
mkdir d:\bin
copy src\command\kernel.ovl d:\ > nul
REM a:
REM cd \bin
REM c:
copy src\command\*. d:\bin
REM Fonts to disk d:
copy src\command\*.fnt d:\bin
REM
REM Man
mkdir d:\usr
mkdir d:\usr\man
copy man\*.* d:\usr\man
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
