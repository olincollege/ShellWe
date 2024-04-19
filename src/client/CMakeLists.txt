add_library(client_receiver receiver.c receiver.h)
add_executable(ChatClient main.c)
target_link_libraries(ChatClient ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(ChatClient PRIVATE util client_receiver )
