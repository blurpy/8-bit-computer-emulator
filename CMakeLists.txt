cmake_minimum_required(VERSION 3.1)
project(8-bit-computer-emulator)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

add_executable(emulator src/main.cpp src/Emulator.cpp src/Emulator.h src/Register.cpp src/Register.h src/Bus.cpp src/Bus.h src/Utils.cpp src/Utils.h src/ArithmeticLogicUnit.cpp src/ArithmeticLogicUnit.h src/Clock.cpp src/Clock.h src/ClockListener.h src/RandomAccessMemory.cpp src/RandomAccessMemory.h src/MemoryAddressRegister.cpp src/MemoryAddressRegister.h src/ProgramCounter.cpp src/ProgramCounter.h src/InstructionRegister.cpp src/InstructionRegister.h src/OutputRegister.cpp src/OutputRegister.h)
target_link_libraries(emulator ${CMAKE_THREAD_LIBS_INIT})
