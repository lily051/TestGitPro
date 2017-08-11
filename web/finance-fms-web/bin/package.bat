@echo off
echo [INFO] Subwcrev the version in version.js.

call version.bat

echo [INFO] Package the war in target dir.

cd %~dp0
cd ..

set MAVEN_OPTS=%MAVEN_OPTS% -Xms512m -Xmx1024m
call mvn clean package -Dmaven.test.skip=true -Dmaven.javadoc.skip=true -X
cd bin
pause