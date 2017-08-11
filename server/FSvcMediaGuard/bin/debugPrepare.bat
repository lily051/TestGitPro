mkdir Debug
set srcdir=.\Release
copy %srcdir%\* .\Debug\ /Y

REM rem   hlog
REM set srcdir=..\..\hiklib\hlog
REM copy %srcdir%\*.dll .\Debug\ /Y
REM copy %srcdir%\*.pdb .\Debug\ /Y
REM rem   hpr
REM set srcdir=..\..\hiklib\hpr\lib\x86_vc60
REM copy %srcdir%\*.dll .\Debug\ /Y
REM copy %srcdir%\*.pdb .\Debug\ /Y
rem   mysql
set srcdir=..\..\myextlib\mysql\mysql_5_1_45\lib\debug
copy %srcdir%\libmysql.dll .\Debug\ /Y
copy %srcdir%\libmysql.pdb .\Debug\ /Y
REM rem   sqlite
REM set srcdir=..\..\myextlib\sqlite\sqlite3
REM copy %srcdir%\*.dll .\Debug\ /Y
REM copy %srcdir%\*.pdb .\Debug\ /Y
rem   theService
REM set srcdir=..\..\..\..\IVMS8100_V3.0\theService\bin
REM copy %srcdir%\*.bat .\Debug\ /Y
REM copy %srcdir%\theService.exe .\Debug\FSvcTransport.exe /Y
REM copy %srcdir%\theService.pdb .\Debug\ /Y
REM copy %srcdir%\theService.log4cxx .\Debug\ /Y

pause