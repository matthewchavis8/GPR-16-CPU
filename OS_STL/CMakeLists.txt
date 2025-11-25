cmake_minimum_required(VERSION 3.15)

project(
  OS_STL
  VERSION 0.0.1
  LANGUAGES CXX
)

if(NOT DEFINED CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 17)
endif()

# Export ClangD
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Adding submodules to link
add_subdirectory(Modules/Math)
add_subdirectory(Modules/String)
add_subdirectory(Modules/Screen)
add_subdirectory(Modules/Keyboard)
add_subdirectory(Modules/Memory)
add_subdirectory(Modules/Sys)

add_library(OS_STL
  INTERFACE
)

# Linking static libs to executable
target_link_libraries(OS_STL INTERFACE
  Math
  String
  Array
  Screen
  Keyboard
  Memory
  Sys
)

# Google Test
include(CTest)

if(BUILD_TESTING)
  # For MSVC compilers avoid conflict
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
  )
  FetchContent_MakeAvailable(googletest)

  add_subdirectory(Test)
endif()
