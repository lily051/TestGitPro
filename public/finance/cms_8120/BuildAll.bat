@echo off
echo %~dp0

set build_config=$ALL

rem set vs2008 cli environment
call "%VS90COMNTOOLS%VSVARS32.BAT"

rem ================== Common =========================
rem HPP,cms_8120
vcbuild /rebuild .\cms_8120.sln %build_config%

pause;
