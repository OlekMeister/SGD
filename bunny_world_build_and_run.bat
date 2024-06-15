@echo off
echo Kompilowanie programu...
mkdir build 2>nul
cd build
cmake ..
if %errorlevel% neq 0 (
    echo Kompilacja nie powiodła się!
    pause
    exit /b %errorlevel%
)
cmake --build .
if %errorlevel% neq 0 (
    echo Kompilacja nie powiodła się!
    pause
    exit /b %errorlevel%
)
echo Kompilacja zakończona sukcesem.
cd ..

echo Kopiowanie plików...
copy SDL2_Libraries\*.dll build\
copy SDL2_Libraries\arial.ttf build\
xcopy /E /I /Y pictures build\pictures\
xcopy /E /I /Y sound build\sound\

echo Uruchamianie programu...
cd build
BunnyWorld.exe
pause
