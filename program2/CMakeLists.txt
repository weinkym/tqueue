# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (program2)

ADD_DEFINITIONS(-std=c++11)

aux_source_directory(../src/ DIR_SRCS)

# 指定生成目标
add_executable(program2 ../src/main.cpp)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)