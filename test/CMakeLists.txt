add_executable(test_util test_util.c)
target_link_libraries(test_util
        PRIVATE util
        PUBLIC ${CRITERION}
)
add_test(
        NAME test_util
        COMMAND test_util ${CRITERION_FLAGS}
)

add_executable(test_server test_server.c)
target_link_libraries(test_server
        PRIVATE server util
        PUBLIC ${CRITERION}
)
add_test(
        NAME test_server
        COMMAND test_server ${CRITERION_FLAGS}
)
