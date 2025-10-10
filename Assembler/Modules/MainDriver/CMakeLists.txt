add_library(MainDriver STATIC mainDriver.cpp)

target_link_libraries(MainDriver
  PUBLIC
    Parser
    Code
    SymbolTable
)
