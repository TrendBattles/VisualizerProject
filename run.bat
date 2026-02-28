@echo off
setlocal

SET "MSYS2_PATH=C:\msys2"

:: Check if the path is actually correct this time
if not exist "%MSYS2_PATH%\msys2_shell.cmd" (
    echo [ERROR] Could not find msys2_shell.cmd in %MSYS2_PATH%
    echo Please verify your MSYS2 installation folder.
    pause
    exit /b
)

:: PREPARE THE PROJECT DIRECTORY
SET "PROJECT_DIR=%~dp0"
echo [INFO] Working Directory: %PROJECT_DIR%
echo [INFO] MSYS2 Shell: "%MSYS2_PATH%\msys2_shell.cmd"

:: LAUNCH AND BUILD
"%MSYS2_PATH%\msys2_shell.cmd" -defterm -no-start -ucrt64 -where "%PROJECT_DIR%" -shell bash -c ^ "rm -rf build && mkdir build && cd build && cmake .. -G 'Unix Makefiles' && make && ./DSVisualizer.exe"

if %ERRORLEVEL% neq 0 (
    echo.
    echo [ERROR] The build failed. Check the terminal output above for C++ errors.
)

pause