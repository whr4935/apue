# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# 项目信息
project(algorithm)

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR})
link_directories(${PROJECT_BINARY_DIR/lib})

set(CMAKE_C_FLAGS "-Wall -g")
set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -g")

add_subdirectory(ADT)
add_subdirectory(search)
add_subdirectory(sort)
add_subdirectory(misc)

# 指定生成目标
add_executable(algorithm ${DIR_SRCS})
target_link_libraries(algorithm libADT)
target_link_libraries(algorithm libsearch)
target_link_libraries(algorithm libsort)
target_link_libraries(algorithm libmisc)
target_link_libraries(algorithm cunit)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
