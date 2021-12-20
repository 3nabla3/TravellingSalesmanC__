cmake_minimum_required(VERSION 3.20)
project(TravellingSalesmanC__)

set(CMAKE_CXX_STANDARD 23)

if (WIN32)
    link_directories(./glfw-3.3.6.bin.WIN64/lib-vc2022)
    include_directories(./glfw-3.3.6.bin.WIN64/include)
endif()

add_executable(TravellingSalesmanC__ main.cpp PermGen.h Timer.cpp Timer.h MultiThreaded.h PermGen.cpp City.h)

if (UNIX)
    target_link_libraries(TravellingSalesmanC__ glfw GL)
elseif (WIN32)
    target_link_libraries(TravellingSalesmanC__ glfw3.lib Opengl32)
endif()
