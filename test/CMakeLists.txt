include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(include)

add_executable(8bit-tests test_main.cpp core/BusTest.cpp core/FlagsRegisterTest.cpp core/StepCounterTest.cpp core/ProgramCounterTest.cpp core/ArithmeticLogicUnitTest.cpp core/EmulatorIntegrationTest.cpp core/AssemblerTest.cpp core/UtilsTest.cpp core/InstructionDecoderTest.cpp core/RandomAccessMemoryTest.cpp core/MemoryAddressRegisterTest.cpp core/TimeSourceTest.cpp core/ClockTest.cpp)
target_link_libraries(8bit-tests 8bit-core)
