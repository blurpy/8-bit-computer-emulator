list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/sdl2)

find_package(Threads REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_library(8bit-ui Window.cpp Window.h UserInterface.cpp UserInterface.h ValueModel.cpp ValueModel.h)

target_link_libraries(8bit-ui 8bit-core)
target_link_libraries(8bit-ui ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(8bit-ui SDL2::Main SDL2::TTF)
