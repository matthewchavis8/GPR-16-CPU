add_library(Keyboard
  STATIC
  gprKeyboard.cpp
)

target_link_libraries(Keyboard
  PUBLIC
  String
  Memory
)
