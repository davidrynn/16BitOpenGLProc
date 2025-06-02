@echo off
echo Building main application...
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" build\16BitCraft.vcxproj /p:Configuration=Debug
if %ERRORLEVEL% NEQ 0 (
    echo Build failed!
    pause
    exit /b 1
)
echo Running application...
.\build\Debug\16BitCraft.exe 