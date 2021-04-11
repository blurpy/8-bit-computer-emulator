add_subdirectory(core)

add_executable(8bit main.cpp)
target_link_libraries(8bit 8bit-core)
