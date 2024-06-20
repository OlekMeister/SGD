@echo off
echo Kompilowanie programu...
mkdir build
cd build
cmake ..
if %errorlevel% neq 0 goto :cmake_failed
cmake --build .
if %errorlevel% neq 0 goto :build_failed
echo Uruchamianie programu...
.\BunnyWorld.exe 2> error.log
goto :end

:cmake_failed
echo CMake configuration failed!
goto :end

:build_failed
echo Build failed!
goto :end

:end
pause
