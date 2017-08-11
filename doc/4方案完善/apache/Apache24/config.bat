@echo off
CHCP 65001
@echo off
set   APACHE_HOME=%~dp0
echo *******************************************
echo "安装APACHE服务到：%APACHE_HOME%"
echo *******************************************

del %APACHE_HOME%\conf\httpd.conf
set  tmp=%APACHE_HOME%
set  APACHE_ROOT=%tmp:\=/%
set  fn=%APACHE_HOME%\conf\httpd.conf.default

setlocal ENABLEDELAYEDEXPANSION
(for /f "tokens=*" %%i in (%fn%) do (
set s=%%i
set s=!s:ServerRoot=ServerRoot "%APACHE_ROOT%"!
echo !s!))>%APACHE_HOME%\conf\httpd.txt

rename %APACHE_HOME%\conf\httpd.txt httpd.conf