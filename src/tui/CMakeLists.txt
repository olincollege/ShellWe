find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
add_library(tui STATIC tui_handler.c tui_handler.h)
target_link_libraries(tui ${CURSES_LIBRARIES})

