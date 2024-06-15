cmake_minimum_required(VERSION 3.10)
project(BunnyWorld)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB SOURCES "src/*.cpp")

add_executable(BunnyWorld ${SOURCES})

set(SDL2_DIR "${CMAKE_SOURCE_DIR}/SDL2_Libraries")

find_library(SDL2_LIB SDL2 PATHS ${SDL2_DIR})
find_library(SDL2_IMAGE_LIB SDL2_image PATHS ${SDL2_DIR})
find_library(SDL2_MIXER_LIB SDL2_mixer PATHS ${SDL2_DIR})
find_library(SDL2_TTF_LIB SDL2_ttf PATHS ${SDL2_DIR})

include_directories(${SDL2_DIR})

target_link_libraries(BunnyWorld ${SDL2_LIB} ${SDL2_IMAGE_LIB} ${SDL2_MIXER_LIB} ${SDL2_TTF_LIB})

if (WIN32)
    target_link_libraries(BunnyWorld -mwindows)
endif()
