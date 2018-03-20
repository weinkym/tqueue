# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)
# 项目信息
project (program1)

ADD_DEFINITIONS(-DUSE_QUEUE)



ADD_DEFINITIONS(-std=c++11)
aux_source_directory(../src/ DIR_SRCS)

# 指定生成目标
add_executable(program1 ../src/main.cpp)

if(UNIX)
	find_package(Threads)
	target_link_libraries (${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
endif()

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)