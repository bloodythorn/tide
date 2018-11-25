
# Playing Cards
add_executable(test_pcards test_pcards.cpp)
add_dependencies(test_pcards tide)
target_link_libraries(test_pcards tide)

# Chess Tests
add_executable(test_chess test_chess.cpp)
add_dependencies(test_chess tide)
target_link_libraries(test_chess tide)

# Loop Tests
add_executable(test_loop test_loop.cpp)
add_dependencies(test_loop tide)
target_link_libraries(test_loop tide)