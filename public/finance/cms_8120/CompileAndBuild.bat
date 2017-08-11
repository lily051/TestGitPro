@echo off

rem compile proto to cc
call .\proto\build.bat

cd %~dp0
set build_config=$ALL
rem set build_config="Release|Win32"
rem set build_config="Debug|Win32"

rem set vs2008 cli environment
call "%VS90COMNTOOLS%VSVARS32.BAT"

rem ================== Common =========================
rem HPP,cms_8120
vcbuild /rebuild .\cms_8120.sln %build_config%

pause;
