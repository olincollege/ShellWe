add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(util)

add_library(util util/util.c util/util.h)
add_library(server server/client_handler.c server/client_handler.h)

add_executable(ChatServer server/main.c)

target_link_libraries(ChatServer ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ChatServer PRIVATE server util)

add_library(client_receiver client/receiver.c client/receiver.h)

add_executable(ChatClient client/main.c)
target_link_libraries(ChatClient PRIVATE util client_receiver)
