add_library(Sys
  STATIC
  gprSys.cpp
)

target_link_libraries(Sys
  PUBLIC
  Memory
)
