rem 设置定时任务
@echo off
rem 设置定时任务名称
set NAME=daily_db_backup
rem 设置任务执行时间
set TIME=23:00:00
rem 设置任务执行操作
set COMMAND="%~dp0db_bk.bat"

%SystemDrive%
cd %windir%\tasks\
if exist %NAME%.job del %NAME%.job

schtasks /create /tn %NAME% /tr "cmd /c %COMMAND%" /sc daily /st %TIME% /ru system
pause