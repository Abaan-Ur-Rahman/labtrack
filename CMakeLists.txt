cmake_minimum_required(VERSION 3.20)
project(labtrack C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Temporary: main.c still #includes the other .c files directly, so it is
# the only file compiled. The next step gives each module its own header
# and lists every .c file here.
add_executable(labtrack src/main.c)

if(MSVC)
    target_compile_options(labtrack PRIVATE /W4)
else()
    target_compile_options(labtrack PRIVATE -Wall -Wextra -Wpedantic)
endif()