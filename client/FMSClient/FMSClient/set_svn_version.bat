@echo off
cd %~dp0
SubWCRev.exe . .\FMSClient_.rc .\FMSClient.rc
echo ****************************************自动设置SVN版本信息成功***************************************
