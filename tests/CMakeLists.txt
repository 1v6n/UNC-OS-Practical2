enable_testing()

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DIR ${CMAKE_SOURCE_DIR}/tests)

add_executable(${PROJECT_NAME}_tests
    ${TEST_DIR}/tests.c                
    ${SRC_DIR}/commands.c
    ${SRC_DIR}/execution.c
    ${SRC_DIR}/jobs.c
    ${SRC_DIR}/global.c
    ${SRC_DIR}/utils.c
)

set_target_properties(${PROJECT_NAME}_tests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

target_link_libraries(${PROJECT_NAME}_tests PRIVATE unity::unity cjson::cjson)

add_test(NAME ${PROJECT_NAME}_UnitTests COMMAND ${CMAKE_BINARY_DIR}/tests/${PROJECT_NAME}_tests)

set_tests_properties(${PROJECT_NAME}_UnitTests PROPERTIES WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/tests)
