# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)

project ("InfiNum")

if(NOT MSVC)
    add_compile_options(-Wall -Wextra)
    if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        add_compile_options(-Werror)
    endif()
else()
    add_compile_options(/permissive-)
endif()

include_directories("include")

add_executable(Runner "main.cpp")

add_library(InfiNum "src/InfiNum.cpp" "src/add.cpp" "src/multiply.cpp" "src/subtract.cpp")

target_link_libraries(Runner InfiNum)
