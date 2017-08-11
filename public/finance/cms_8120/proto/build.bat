@echo off
cd %~dp0\_cms_8120\include
set cppoutpath=%cd%
del /s *.cc
del /s *.h

::proto
cd ../../
set protodefpath=%cd%
dir *.proto /s/a/b > temp.txt

::public include
cd ../../../base/Google/protobuf/include
set publicincludepath=%cd%

echo "************************************************************************"
echo %protodefpath%
echo %publicincludepath%
echo "************************************************************************"

::tools
cd %protodefpath%
cd ./
for /f "delims=" %%i in (./temp.txt) do (protoc --proto_path="%protodefpath%" --proto_path="%publicincludepath%"  --cpp_out="%cppoutpath%" "%%i")
cd %protodefpath%

pause
