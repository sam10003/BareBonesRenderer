
cmake_minimum_required(VERSION 3.10)

project(WireframeRenderer LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(WireframeRenderer
    src/main.cpp

    src/core/camera.cpp
    src/core/mesh.cpp
    src/core/render.cpp
    src/core/transform.cpp

    src/utils/log.cpp
    src/utils/data_f.cpp

    src/platform/run_loop.cpp 
    src/platform/window.cpp 
)

target_include_directories(WireframeRenderer PRIVATE include)

if(MINGW)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
    target_link_libraries(WireframeRenderer PRIVATE pthread)
endif()
