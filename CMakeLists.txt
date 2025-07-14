cmake_minimum_required(VERSION 3.15)
project(MobPaint C)

set(CMAKE_C_STANDARD 11)

# Enable strict warnings
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES
    src/*.c
    src/tools/*.c
    src/context/*.c
)

# Find SDL2 and SDL2_gfx
find_package(SDL2 REQUIRED)
find_path(SDL2_GFX_INCLUDE_DIR SDL2_gfxPrimitives.h)
find_library(SDL2_GFX_LIBRARY NAMES SDL2_gfx)

# Find cJSON
find_path(CJSON_INCLUDE_DIR cjson/cJSON.h)
find_library(CJSON_LIBRARY NAMES cjson)

# Include dirs
include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_GFX_INCLUDE_DIR}
    ${CJSON_INCLUDE_DIR}
)

# Define executable
add_executable(mobpaint ${SOURCES})

# Link libraries
target_link_libraries(mobpaint
    ${SDL2_LIBRARIES}
    ${SDL2_GFX_LIBRARY}
    ${CJSON_LIBRARY}
    pthread
)
