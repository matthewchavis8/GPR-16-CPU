add_library(
  CompilerAnalyzer
  STATIC
  compilerAnalyzer.cpp
)

target_link_libraries(CompilerAnalyzer
  PUBLIC
  CompilationEngine
)
