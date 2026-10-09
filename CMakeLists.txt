cmake_minimum_required(VERSION 3.20)
project(labtrack C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable(labtrack
    src/main.c
    src/admin.c
    src/technician.c
    src/student.c
    src/department.c
)
target_include_directories(labtrack PRIVATE include)

if(MSVC)
    target_compile_options(labtrack PRIVATE /W4)
else()
    target_compile_options(labtrack PRIVATE -Wall -Wextra -Wpedantic)
endif()
