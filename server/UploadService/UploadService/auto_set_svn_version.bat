@echo off
cd %~dp0
SubWCRev.exe . .\UploadService.rc_ .\UploadService.rc
echo ****************************************自动设置SVN版本信息成功***************************************
