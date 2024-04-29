find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
add_library(tui STATIC window_handler.c window_handler.h)
target_link_libraries(tui ${CURSES_LIBRARIES})

