rem ���ö�ʱ����
@echo off
rem ���ö�ʱ��������
set NAME=daily_db_backup
rem ��������ִ��ʱ��
set TIME=23:00:00
rem ��������ִ�в���
set COMMAND="%~dp0db_bk.bat"

%SystemDrive%
cd %windir%\tasks\
if exist %NAME%.job del %NAME%.job

schtasks /create /tn %NAME% /tr "cmd /c %COMMAND%" /sc daily /st %TIME% /ru system
pause