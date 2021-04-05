include_directories (${PROJECT_SOURCE_DIR}/src)

add_executable(8bit-tests test_main.cpp BusTest.cpp FlagsRegisterTest.cpp StepCounterTest.cpp)
target_link_libraries(8bit-tests 8bit-lib)
