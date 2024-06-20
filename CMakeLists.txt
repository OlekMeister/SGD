cmake_minimum_required(VERSION 3.10)

project(BunnyWorld)

set(CMAKE_CXX_STANDARD 17)

# Ustawienie zmiennych dla bibliotek SDL
set(SDL2_DIR "C:/msys64/mingw64")
set(SDL2_IMAGE_DIR "C:/msys64/mingw64")
set(SDL2_MIXER_DIR "C:/msys64/mingw64")
set(SDL2_TTF_DIR "C:/msys64/mingw64")

# Znalezienie bibliotek SDL
find_library(SDL2_LIB SDL2 PATHS ${SDL2_DIR}/lib)
find_library(SDL2MAIN_LIB SDL2main PATHS ${SDL2_DIR}/lib)
find_library(SDL2_IMAGE_LIB SDL2_image PATHS ${SDL2_IMAGE_DIR}/lib)
find_library(SDL2_MIXER_LIB SDL2_mixer PATHS ${SDL2_MIXER_DIR}/lib)
find_library(SDL2_TTF_LIB SDL2_ttf PATHS ${SDL2_TTF_DIR}/lib)

# Dołączenie katalogów nagłówków
include_directories(${SDL2_DIR}/include ${SDL2_IMAGE_DIR}/include ${SDL2_MIXER_DIR}/include ${SDL2_TTF_DIR}/include)

# Dodanie plików źródłowych
set(SOURCES
        src/Main.cpp
        src/Player.cpp
        src/Platform.cpp
)

# Utworzenie pliku wykonywalnego
add_executable(BunnyWorld ${SOURCES})

# Połączenie bibliotek SDL z projektem
target_link_libraries(BunnyWorld PRIVATE ${SDL2_LIB} ${SDL2MAIN_LIB} ${SDL2_IMAGE_LIB} ${SDL2_MIXER_LIB} ${SDL2_TTF_LIB} -lmingw32 -lSDL2 -lSDL2main)
