add_library(Screen
  STATIC
  gprScreen.cpp
)

target_link_libraries(Screen
  PUBLIC
  Memory
  Math
)
