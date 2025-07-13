cmake_minimum_required(VERSION 3.15)
project(MobPaint C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

set(SOURCES
        src/main.c
        src/app.c
        src/sidebar.c
        src/tools/tools.c
        src/context/logs.c
        src/context/history.c
        src/context/paint_context.c
        src/context/config.c
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(mobpaint ${SOURCES})

target_link_libraries(mobpaint ${SDL2_LIBRARIES} pthread cjson)

if (MSVC)
    target_compile_options(mobpaint PRIVATE /W4 /WX)
else()
    target_compile_options(mobpaint PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
