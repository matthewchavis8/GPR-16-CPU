add_library(
  VMTranslator
  STATIC
  vmtranslator.cpp
)

target_link_libraries(VMTranslator
  PUBLIC
    Parser
    CodeWriter
)
