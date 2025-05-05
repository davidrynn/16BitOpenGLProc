@echo off
REM Save the current directory
set CURRENT_DIR=%cd%

REM Navigate to the build directory
cd build

REM Run CMake to configure and build the project
cmake --build . --config Debug
if %errorlevel% neq 0 (
    echo Build failed. Exiting.
    cd %CURRENT_DIR%
    exit /b %errorlevel%
)

REM Run the tests using ctest
ctest --output-on-failure
if %errorlevel% neq 0 (
    echo Tests failed. Exiting.
    cd %CURRENT_DIR%
    exit /b %errorlevel%
)

REM Return to the original directory
cd %CURRENT_DIR%
echo Tests completed successfully.