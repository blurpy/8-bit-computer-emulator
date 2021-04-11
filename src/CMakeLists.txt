find_package(Threads REQUIRED)

add_library(8bit-lib Emulator.cpp Emulator.h GenericRegister.cpp GenericRegister.h Bus.cpp Bus.h Utils.cpp Utils.h ArithmeticLogicUnit.cpp ArithmeticLogicUnit.h Clock.cpp Clock.h ClockListener.h ClockListener.cpp RandomAccessMemory.cpp RandomAccessMemory.h MemoryAddressRegister.cpp MemoryAddressRegister.h ProgramCounter.cpp ProgramCounter.h InstructionRegister.cpp InstructionRegister.h OutputRegister.cpp OutputRegister.h StepCounter.cpp StepCounter.h InstructionDecoder.cpp InstructionDecoder.h StepListener.h Assembler.cpp Assembler.h RegisterListener.h FlagsRegister.cpp FlagsRegister.h Instructions.cpp Instructions.h)

add_executable(8bit main.cpp)
target_link_libraries(8bit 8bit-lib)
target_link_libraries(8bit-lib ${CMAKE_THREAD_LIBS_INIT})
