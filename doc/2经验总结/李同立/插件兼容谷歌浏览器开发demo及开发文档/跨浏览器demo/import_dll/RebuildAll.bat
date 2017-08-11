echo off


rem set environment variables:
set _devenv="%VS90COMNTOOLS%..\..\Common7\IDE\devenv.com"
rem set compile log
set _log="%~dp0compileResults.log"
rem set _solution_file="%~dp0t33.sln"
set _solution_file="%~dp0t33\t33.vcproj"
set buildAnyCPU="x86"

echo [%DATE% %Time%] Start compile sequence >%_log%
echo Used compile configuration is %buildAnyCPU% >>%_log%

rem Start compile************************************************
%_devenv% %_solution_file% /Rebuild "Release|Win32" /Out %_log%

if not %errorlevel% == 0 echo %_solution_file% failed!   Error: %errorlevel% >>%_log%
if %errorlevel% == 0 echo %_solution_file% compiled successful >>%_log%

rem If compile failed stop processing:
if not %errorlevel% == 0 pause
echo [%DATE% %Time%] Finished compile sequence >>%_log%

pause     
