add_library(server client_handler.c client_handler.h)
add_executable(ChatServer main.c)
target_link_libraries(ChatServer ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ChatServer PRIVATE server util)
