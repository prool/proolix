@echo off
cls
echo ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
echo ³ ˆ­áâ ««ïæ¨ï Ž‘ Proolix ­  ¤¨áª¥âã a:                               ³
echo ³                                                                    ³
echo ³ Š â «®£, ¢ ª®â®à®¬ à á¯ ª®¢ ­ ä ©« slime?.arj                      ³
echo ³ ¤®«¦¥­ ¡ëâì â¥ªãé¨¬.                                               ³
echo ³                                                                    ³
echo ³ “çâ¨â¥, ¡ãâ-á¥ªâ®à ¤¨áª¥âë ¨ ¢áï                                   ³
echo ³ ¨­ä®à¬ æ¨ï, á®¤¥à¦ é ïáï ­  ¤¨áª¥â¥, ¬®£ãâ ¡ëâì § ¯®àç¥­ë !!!      ³
echo ³  ¦¬¨â¥ CTRL BREAK, ¥á«¨ ‚ë ¯¥à¥¤ã¬ «¨ á¢ï§ë¢ âìáï á Proolix'®¬.   ³
echo ³ ‚ ¯à®â¨¢­®¬ á«ãç ¥ ­ ¦¬¨â¥ ANY KEY                                 ³
echo ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
pause>nul
cls
src\boot\bp -d0 -i -y src\boot\boots.bin
if errorlevel 1 goto Error
echo Boot manager to diskette a:
copy src\bm\bootf.bin a:\boot
echo Kernel to diskette a:
copy src\kernel\kernel.bin a:\kernel
echo on
REM
REM Files description to diskette a:
copy files.bbs a:\
REM
REM Utilities to diskette a:
mkdir a:\bin
copy src\command\kernel.ovl a:\
REM a:
REM cd \bin
REM c:
copy src\command\*. a:\bin
REM Fonts to diskette a:
copy src\command\*.fnt a:\bin
del a:\bin\makefile
REM Man to diskette
mkdir a:\usr
mkdir a:\usr\man
copy man\*.* a:\usr\man
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