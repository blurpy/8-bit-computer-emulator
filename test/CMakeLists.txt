include_directories (${PROJECT_SOURCE_DIR}/src)

add_executable(8bit-tests BusTest.cpp)
target_link_libraries(8bit-tests 8bit-lib)
