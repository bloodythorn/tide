
# Playing Cards
add_executable(test_cards test_cards.cpp)
add_dependencies(test_cards tide)
target_link_libraries(test_cards tide)

# Chess Tests
add_executable(test_chess test_chess.cpp)
add_dependencies(test_chess tide)
target_link_libraries(test_chess tide)

# Chess Tests
add_executable(test_checkers test_checkers.cpp)
add_dependencies(test_checkers tide)
target_link_libraries(test_checkers tide)

# Loop Tests
add_executable(test_loop test_loop.cpp)
add_dependencies(test_loop tide)
target_link_libraries(test_loop tide)

# CLI Platform Test
add_executable(test_p_CLI test_p_CLI.cpp)
add_dependencies(test_p_CLI tide)
target_link_libraries(test_p_CLI tide)
