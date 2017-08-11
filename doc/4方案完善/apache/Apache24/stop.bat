set   APACHE_HOME=%~dp0
set   "HTTPD=%APACHE_HOME%\bin\httpd.exe"
echo  "APACHE_HOME-%APACHE_HOME%"
echo  "HTTPD-%HTTPD%"
"%HTTPD%" -k stop -n "Apache Http Server"
pause