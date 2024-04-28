find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
add_executable(tui tui.c)
target_link_libraries(tui ${CURSES_LIBRARIES})
