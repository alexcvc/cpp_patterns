set(TEST_TARGET_NAME test_doctest)

set(${TEST_TARGET_NAME}_SRC main.cpp)

find_package(Threads REQUIRED)

add_executable(${TEST_TARGET_NAME} ${${TEST_TARGET_NAME}_SRC})
target_link_libraries(${TEST_TARGET_NAME} Threads::Threads)
target_include_directories(${TEST_TARGET_NAME} PRIVATE
        ${Doctest_INCLUDE_DIR}
        ${Spdlog_INCLUDE_DIR}
        ${Fmt_INCLUDE_DIR}
        )
add_test(NAME ${TEST_TARGET_NAME}
        COMMAND ${TEST_TARGET_NAME}
        WORKING_DIRECTORY $<TARGET_FILE_DIR:${TEST_TARGET_NAME}>)