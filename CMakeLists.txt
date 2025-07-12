cmake_minimum_required(VERSION 3.15)
project(MobPaint C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(mobpaint
        src/main.c
        src/logs.c
)
target_link_libraries(mobpaint SDL2)