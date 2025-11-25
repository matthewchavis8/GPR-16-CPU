add_library(Screen
  STATIC
  gprScreen.cpp
)

target_link_libraries(Screen
  Memory
)
