@echo off
set logfile=aa.log
cd %~dp0
echo µ±Ç°Ä¿Â¼£º%cd%
set build_config="Release|Win32"

rem set vs2008 cli environment
call "%VS90COMNTOOLS%VSVARS32.BAT"

rem ================== Common =========================
call .\auto_set_svn_version.bat

vcbuild /rebuild .\ConfigTool\ConfigTool.vcproj %build_config%

pause
