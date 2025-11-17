cmake_minimum_required(VERSION 3.15)

project(
  Compiler
  VERSION 0.0.1
  LANGUAGES CXX
)

if(NOT DEFINED CMAKE_CXX_STANDARD)
	set(CMAKE_CXX_STANDARD 17)
endif()

# Export ClangD
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Adding submodules to link
add_subdirectory(Modules/CompilationEngine)
add_subdirectory(Modules/CompilerAnalyzer)
add_subdirectory(Modules/SymbolTable)
add_subdirectory(Modules/Tokenizer)
add_subdirectory(Modules/VMWriter)


add_executable(
  Compiler
  compiler.cpp
)

# Linking static libs to executable
target_link_libraries(Compiler PRIVATE
  CompilerAnalyzer
  SymbolTable
  VMWriter
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
