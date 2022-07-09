cmake_minimum_required(VERSION 3.5)

project(HandsOnDesignPatterns LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

function(add_files VAR DIR)
  set(files ${${VAR}})
  foreach (file ${ARGN})
    set(files ${files} ${DIR}/${file})
  endforeach()
  set(${VAR} ${files} PARENT_SCOPE)
endfunction()

# Testing library
include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

add_subdirectory(Chapter04)
