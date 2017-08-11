@echo off
set REDIS_HOME=%~dp0
set "CONFIG_FILE=%REDIS_HOME%\redis.windows-service.conf"
set "REDIS_SERVER=%REDIS_HOME%\redis-server.exe"

%REDIS_SERVER% --service-install "%CONFIG_FILE%" --service-name FsvcRedis --maxheap 1024m
pause