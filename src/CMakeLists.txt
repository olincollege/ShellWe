add_library(util util.c util.h)

add_executable(main main.c)
target_link_libraries(main PRIVATE util)