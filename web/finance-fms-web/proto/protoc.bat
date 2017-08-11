@echo off
:: 进入proto文件输出位置的路径
cd %~dp0\output\java	
:: 把这个proto的输出路径赋值给--javaoutpath
set javaoutpath=%cd%
:: 从所有子目录删除指定文件
del /s *.java

::proto
cd ../../
cd protoc/
:: proto的源文件路径
set protodefpath=%cd%
:: 把proto文件的路径，以及文件的名字都复制到 protoc_log.txt文档中
dir *.proto /s/a/b > ../log/protoc_log.txt

::protode
echo "************************************************************************"
echo  %javaoutpath%
echo "************************************************************************"

cd %protodefpath%
cd ./
:: 循环protoc_log.txt文档中的目录，然后通过命令protoc 命令来把proto文件生成对应的java文件。
for /f "delims=" %%i in (../log/protoc_log.txt) do (protoc -I=/ --proto_path="%protodefpath%" --java_out="%javaoutpath%" "%%i")
cd %protodefpath%

pause



