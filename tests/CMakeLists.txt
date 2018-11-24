
# Playing Cards
add_executable(playingcards playingcards.cpp)
add_dependencies(playingcards tide)
target_link_libraries(playingcards tide)