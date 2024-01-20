@echo off
if "%1" == "-a"	goto exec
if "%1" == "-d"	goto exec
if "%1" == "-r"	goto exec
if "%1" == ""	goto usage

:usage
echo Usage: command_build.bat [-d] [-r] [-a]
echo Option:
echo    -d	Debug build.
echo    -r	Release build.
echo    -a	All build.
goto :end

:exec
rem https://learn.microsoft.com/ja-jp/visualstudio/ide/reference/command-prompt-powershell?view=vs-2022
call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"
if %1 == -a goto all
if %1 == -d goto debug
if %1 == -r goto release

:all
:debug
MSBuild Oocfuu.sln /t:build /p:Configuration=Debug;Platform="x64" -fileLogger
if not %1 == -a goto end
:release
MSBuild Oocfuu.sln /t:build /p:Configuration=Release;Platform="x64"

:end