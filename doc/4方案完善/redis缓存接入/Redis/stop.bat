@echo off
set REDIS_HOME=%~dp0
set "REDIS_SERVER=%REDIS_HOME%\redis-server.exe"

%REDIS_SERVER% --service-stop --service-name FsvcRedis
pause