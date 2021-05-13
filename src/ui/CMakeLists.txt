find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})

add_library(8bit-ui Window.cpp Window.h UserInterface.cpp UserInterface.h)

target_link_libraries(8bit-ui 8bit-core)
target_link_libraries(8bit-ui ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(8bit-ui ${SDL2_LIBRARIES})
