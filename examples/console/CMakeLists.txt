
# Console
add_executable(console
  console.cpp
  state_console.cpp)
add_dependencies(console tide)
target_link_libraries(console tide)