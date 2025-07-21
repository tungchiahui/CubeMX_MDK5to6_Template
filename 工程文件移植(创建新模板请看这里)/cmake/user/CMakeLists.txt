cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C ASM)

# 添加用户自己的头文件目录
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../applications/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/../../bsp/boards/Inc
)


set(Applications_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../applications/Src/cpp_interface.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../applications/Src/xxx.cpp
)

set(Bsp_Boards_Src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../bsp/boards/Src/bsp_delay.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/../../bsp/boards/Src/xxx.cpp
)

target_sources(${CMAKE_PROJECT_NAME} PRIVATE 
    ${Applications_Src}
    ${Bsp_Boards_Src}
    )
