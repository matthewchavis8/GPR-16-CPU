add_library(
  CompilationEngine
  STATIC
  compileEngine.cpp
)

target_link_libraries(CompilationEngine
  PUBLIC
  Tokenizer
  VMWriter
  SymbolTable
)
