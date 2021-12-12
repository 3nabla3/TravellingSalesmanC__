cmake_minimum_required(VERSION 3.20)
project(TravellingSalesmanC__)

set(CMAKE_CXX_STANDARD 23)
include_directories(./glfw-3.3.5.bin.WIN64/include)
link_directories(./glfw-3.3.5.bin.WIN64/lib-vc2022)

add_executable(TravellingSalesmanC__ main.cpp PermGen.h Timer.cpp Timer.h MultiThreaded.h PermGen.cpp City.h "PermInfo.h")
target_link_libraries(TravellingSalesmanC__ glfw3 Opengl32)
