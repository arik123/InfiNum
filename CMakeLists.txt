# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.20)
set(CMAKE_CXX_STANDARD 17)

project ("InfiNum")
include(CTest)

if(NOT MSVC)
    add_compile_options(-Wall -Wextra)
    if(PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        add_compile_options(-Werror)
    endif()
else()
    add_compile_options(/permissive-)
endif()

include_directories("include")


add_library(InfiNum "src/InfiNum.cpp" "src/add.cpp" "src/multiply.cpp" "src/subtract.cpp" "src/div.cpp" "src/stringOP.cpp")

create_test_sourcelist (Tests
  "tests/test.cxx"
  "test/defaultConstructor.cxx"
  "test/stringConstructor.cxx"
  "test/mcConstructor.cxx"
  "test/add.cxx"
  )
add_executable (CommonCxxTests ${Tests})
target_link_libraries(CommonCxxTests InfiNum)

# remove the test driver source file
set (TestsToRun ${Tests})
remove (TestsToRun "tests/test.cxx")

# Add all the ADD_TEST for each test
foreach (test ${TestsToRun})
  cmake_path(GET test EXTENSION TEST_FILE_EXTENSION)
  string(REPLACE "${TEST_FILE_EXTENSION}" "" TName "${test}")
  message("add test: ${TName}")
  add_test (NAME ${TName} COMMAND CommonCxxTests ${TName})
endforeach ()