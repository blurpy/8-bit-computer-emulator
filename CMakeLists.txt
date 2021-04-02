cmake_minimum_required(VERSION 3.1)
project(8-bit-computer-emulator)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

add_executable(8bit src/main.cpp src/Emulator.cpp src/Emulator.h src/GenericRegister.cpp src/GenericRegister.h src/Bus.cpp src/Bus.h src/Utils.cpp src/Utils.h src/ArithmeticLogicUnit.cpp src/ArithmeticLogicUnit.h src/Clock.cpp src/Clock.h src/ClockListener.h src/ClockListener.cpp src/RandomAccessMemory.cpp src/RandomAccessMemory.h src/MemoryAddressRegister.cpp src/MemoryAddressRegister.h src/ProgramCounter.cpp src/ProgramCounter.h src/InstructionRegister.cpp src/InstructionRegister.h src/OutputRegister.cpp src/OutputRegister.h src/StepCounter.cpp src/StepCounter.h src/InstructionDecoder.cpp src/InstructionDecoder.h src/StepListener.h src/StepListener.cpp src/Assembler.cpp src/Assembler.h src/RegisterListener.cpp src/RegisterListener.h src/FlagsRegister.cpp src/FlagsRegister.h)
target_link_libraries(8bit ${CMAKE_THREAD_LIBS_INIT})
