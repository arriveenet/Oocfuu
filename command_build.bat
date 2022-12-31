@echo off
rem https://learn.microsoft.com/ja-jp/visualstudio/ide/reference/command-prompt-powershell?view=vs-2022
call "%ProgramFiles%\Microsoft Visual Studio\2022\Community\Common7\Tools\VsDevCmd.bat"

MSBuild Oocfuu.sln /t:build /p:Configuration=Debug;Platform="x64" -fileLogger
rem MSBuild Oocfuu.sln /t:build /p:Configuration=Release;Platform="x64"

pause