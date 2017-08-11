rem   hlog
set srcdir=..\..\hiklib\hlog
copy %srcdir%\*.dll .\Release\ /Y
copy %srcdir%\*.pdb .\Release\ /Y
rem   hpr
set srcdir=..\..\hiklib\hpr\lib\x86_vc60
copy %srcdir%\*.dll .\Release\ /Y
copy %srcdir%\*.pdb .\Release\ /Y
rem   mysql
set srcdir=..\..\myextlib\mysql\mysql_5_1_45\lib\opt
copy %srcdir%\libmysql.dll .\Release\ /Y
copy %srcdir%\libmysql.pdb .\Release\ /Y
rem   sqlite
set srcdir=..\..\myextlib\sqlite\sqlite3
copy %srcdir%\*.dll .\Release\ /Y
copy %srcdir%\*.pdb .\Release\ /Y
rem   theService
set srcdir=..\..\..\..\IVMS8100_V3.0\theService\bin
copy %srcdir%\*.bat .\Release\ /Y
copy %srcdir%\theService.exe .\Release\FSvcRecGuard.exe /Y
copy %srcdir%\theService.pdb .\Release\ /Y
copy %srcdir%\theService.log4cxx .\Release\ /Y

pause