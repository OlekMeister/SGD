@echo off
setlocal

echo Kompilowanie programu...
g++ main.cpp Player.cpp Platform.cpp -o BunnyWorld -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
if %errorlevel% neq 0 (
    echo Kompilacja nie powiodła się!
    pause
    endlocal
    exit /b %errorlevel%
)

echo Kompilacja zakończona sukcesem.

echo Uruchamianie programu...
set PATH=%PATH%;%~dp0SDL2_Libraries
BunnyWorld.exe

endlocal
pause
