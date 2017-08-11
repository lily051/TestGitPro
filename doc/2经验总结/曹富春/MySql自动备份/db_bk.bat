@echo off
rem 创建存放备份文件的目录db_bk
set db_back_path="%~dp0..\db_bk"
if not exist "%db_back_path%" md "%db_back_path%"

rem 备份文件名称，文件名为当前日期如ivms8000fms20160718.sql
set filename="%date:~0,4%%date:~5,2%%date:~8,2%"
rem 使用mysqldump.exe备份数据库，命令参数： -u数据库用户名、-p数据库密码 数据库名称 >备份文件名称
"%~dp0\mysqldump.exe" -u root -p888 ivms8000fms>"%db_back_path%\ivms8000fms%filename%.sql"

rem 获取10天前的日期
set DaysAgo=10
call :DateToDays %date:~0,4% %date:~5,2% %date:~8,2% PassDays
set /a PassDays-=%DaysAgo%
call :DaysToDate %PassDays% DstYear DstMonth DstDay
set DstDate=%DstYear%%DstMonth%%DstDay%

rem 删除10天前文件
echo "%db_back_path%\ivms8000fms%DstDate%.sql"
if exist "%db_back_path%\ivms8000fms%DstDate%.sql" del "%db_back_path%\ivms8000fms%DstDate%.sql"

rem 日期转天数
:DateToDays %yy% %mm% %dd% days
setlocal ENABLEEXTENSIONS
set yy=%1&set mm=%2&set dd=%3
if 1%yy% LSS 200 if 1%yy% LSS 170 (set yy=20%yy%) else (set yy=19%yy%)
set /a dd=100%dd%%%100,mm=100%mm%%%100
set /a z=14-mm,z/=12,y=yy+4800-z,m=mm+12*z-3,j=153*m+2
set /a j=j/5+dd+y*365+y/4-y/100+y/400-2472633
endlocal&set %4=%j%&goto :EOF

rem 天数转日期
:DaysToDate %days% yy mm dd
setlocal ENABLEEXTENSIONS
set /a a=%1+2472632,b=4*a+3,b/=146097,c=-b*146097,c/=4,c+=a
set /a d=4*c+3,d/=1461,e=-1461*d,e/=4,e+=c,m=5*e+2,m/=153,dd=153*m+2,dd/=5
set /a dd=-dd+e+1,mm=-m/10,mm*=12,mm+=m+3,yy=b*100+d-4800+m/10
(if %mm% LSS 10 set mm=0%mm%)&(if %dd% LSS 10 set dd=0%dd%)
endlocal&set %2=%yy%&set %3=%mm%&set %4=%dd%&goto :EOF