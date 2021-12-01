cmake_minimum_required(VERSION 3.20)
project(TravellingSalesmanC__)

set(CMAKE_CXX_STANDARD 23)

add_executable(TravellingSalesmanC__ main.cpp PermGen.h Timer.cpp Timer.h MultiThreaded.h PermGen.cpp City.h)
target_link_libraries(TravellingSalesmanC__ glfw GL pthread)
