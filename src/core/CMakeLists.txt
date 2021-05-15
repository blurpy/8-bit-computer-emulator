find_package(Threads REQUIRED)

add_library(8bit-core Emulator.cpp Emulator.h GenericRegister.cpp GenericRegister.h Bus.cpp Bus.h Utils.cpp Utils.h ArithmeticLogicUnit.cpp ArithmeticLogicUnit.h Clock.cpp Clock.h ClockListener.h RandomAccessMemory.cpp RandomAccessMemory.h MemoryAddressRegister.cpp MemoryAddressRegister.h ProgramCounter.cpp ProgramCounter.h InstructionRegister.cpp InstructionRegister.h OutputRegister.cpp OutputRegister.h StepCounter.cpp StepCounter.h InstructionDecoder.cpp InstructionDecoder.h StepListener.h Assembler.cpp Assembler.h RegisterListener.h FlagsRegister.cpp FlagsRegister.h Instructions.cpp Instructions.h TimeSource.cpp TimeSource.h ValueObserver.h ArithmeticLogicUnitObserver.h ClockObserver.h FlagsRegisterObserver.h InstructionDecoderObserver.h Disassembler.cpp Disassembler.h)

target_link_libraries(8bit-core ${CMAKE_THREAD_LIBS_INIT})
