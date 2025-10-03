cmake_minimum_required(VERSION 3.15)

project(
  Assembler
  VERSION 0.0.1
  LANGUAGES CXX
)

if(NOT DEFINED CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 17)
endif()

# Export ClangD
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# Adding submodules to link
add_subdirectory(Modules/Parser)
add_subdirectory(Modules/Code)
add_subdirectory(Modules/MainDriver)
add_subdirectory(Modules/SymbolTable)


add_executable(
  Assembler
  assembler.cpp
)

# Linking static libs to executable
target_link_libraries(Assembler PRIVATE
  Parser
  Code
  MainDriver
  SymbolTable
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

  add_subdirectory(test)
endif()
