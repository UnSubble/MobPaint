cmake_minimum_required(VERSION 3.15)
project(MobPaint C)

set(CMAKE_C_STANDARD 11)

# Include header files path
include_directories(include)

# Source files
set(SOURCES
        src/main.c
        src/logs.c
)

# Find SDL2 package
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(mobpaint ${SOURCES})

target_link_libraries(mobpaint ${SDL2_LIBRARIES})

# Enable compiler warnings
if (MSVC)
    target_compile_options(mobpaint PRIVATE /W4 /WX)
else()
    target_compile_options(mobpaint PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()
