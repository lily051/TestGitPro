echo %~dp0

set build_config="Release|Win32"

rem set vs2008 cli environment
call "%VS90COMNTOOLS%VSVARS32.BAT"

vcbuild /rebuild  "%~dp0PlaybackSimpleDll.sln" %build_config%
                
pause;
