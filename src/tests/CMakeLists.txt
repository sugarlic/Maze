AUX_SOURCE_DIRECTORY(./src TEST_SOURCES)
add_executable(TESTING
    ${TEST_SOURCES}
)
AUX_SOURCE_DIRECTORY(./src MAZE_SRCS)
file(GLOB_RECURSE MAZE_HEADERS
    includes/*.h
)

target_link_libraries(TESTING
    ${P_NAME}_Backend
    gtest
)

include(GoogleTest)
gtest_discover_tests(TESTING)
add_test(NAME TestMyCode COMMAND TestMyCode)
