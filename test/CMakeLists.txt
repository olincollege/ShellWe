add_executable(test_util test_util.c)
target_link_libraries(test_util
        PRIVATE util
        PUBLIC ${CRITERION}
)
add_test(
        NAME test_util
        COMMAND test_util ${CRITERION_FLAGS}
)