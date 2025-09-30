add_executable(unit_Parser
  code.cpp
  mainDriver.cpp
  parser.cpp
  symbolTable.cpp
)

target_link_libraries(unit_parser
  PRIVATE
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_parser)
