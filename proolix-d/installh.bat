@echo off
cls
echo ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
echo ³ ˆ­áâ ««ïæ¨ï Ž‘ Proolix ­  a: ¨ c: (boot manager ­  a:, ï¤à® ­  c: )³
echo ³                                                                    ³
echo ³ Installator (install.bat) version H-0.0.2.1 30-Sep-98              ³
echo ³                                                                    ³
echo ³ Š â «®£, ¢ ª®â®à®¬ à á¯ ª®¢ ­ ä ©« slime?.arj                      ³
echo ³ ¤®«¦¥­ ¡ëâì â¥ªãé¨¬.                                               ³
echo ³ ‚ ãáâà®©áâ¢¥ a: ¤®«¦­  ¡ëâì ç¨áâ ï ¤¨áª¥â !                        ³
echo ³                                                                    ³
echo ³ “çâ¨â¥, ¡ãâ-á¥ªâ®àë ¤¨áª  ¨ ¤¨áª¥âë,   â ª¦¥ ¢áï                   ³
echo ³ ¨­ä®à¬ æ¨ï, á®¤¥à¦ é ïáï ­  ¤¨áª¥ ¨ ¤¨áª¥â¥, ¬®£ãâ ¡ëâì § ¯®àç¥­ë !³
echo ³  ¦¬¨â¥ CTRL BREAK, ¥á«¨ ‚ë ¯¥à¥¤ã¬ «¨.                            ³
echo ³ ‚ ¯à®â¨¢­®¬ á«ãç ¥ ­ ¦¬¨â¥ ANY KEY                                 ³
echo ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ
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
