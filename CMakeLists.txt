cmake_minimum_required(VERSION 3.10)
project(BunnyWorld)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_PATH "C:/Users/Aleksander/Desktop/SGD/SDL2-devel-2.30.4-mingw/SDL2-2.30.4/x86_64-w64-mingw32")

include_directories(${SDL2_PATH}/include)
link_directories(${SDL2_PATH}/lib)

add_executable(BunnyWorld src/Main.cpp src/Player.cpp src/Platform.cpp)

target_link_libraries(BunnyWorld
        ${SDL2_PATH}/lib/libSDL2.dll.a
        ${SDL2_PATH}/lib/libSDL2_image.dll.a
        ${SDL2_PATH}/lib/libSDL2_mixer.dll.a
        ${SDL2_PATH}/lib/libSDL2_ttf.dll.a
)

if (WIN32)
    target_link_libraries(BunnyWorld -lmingw32 -lkernel32 -luser32 -lgdi32 -lwinspool -lshell32 -lole32 -loleaut32 -luuid -lcomdlg32 -ladvapi32)
endif()
