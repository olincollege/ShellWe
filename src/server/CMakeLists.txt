add_library(client_handler client_handler.c client_handler.h)

add_library(server server.c server.h)
target_link_libraries(server PRIVATE util)

add_executable(ChatServer main.c)
target_link_libraries(ChatServer ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ChatServer PRIVATE server client_handler util)
