# CMake 最低版本号要求
cmake_minimum_required(VERSION 2.6)
# 项目信息
project(Base)

SET(CMAKE_C_COMPILER g++)
SET(CMAK_CXX_COMPILER g++)

# 添加c++11编译选项
add_compile_options(-std=c++11)

# 显示或关闭编译的详细信息
set(CMAKE_VERBOSE_MAKEFILE ON)
#set(CMAKE_VERBOSE_MAKEFILE OFF)

# DEBUG OR RELEASE
set(CMAKE_BUILD_TYPE DEBUG)
#set(CMAKE_BUILD_TYPE RELEASE)

# 导出编译符号，供vim  YouCompleteMe 使用
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 显示变量
message(STATUS "CMAKE_BUILD_TYPE:" ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_C_COMPILER:" ${CMAKE_C_COMPILER})
message(STATUS "CMAKE_CXX_COMPILER:" ${CMAKE_CXX_COMPILER})


# 设置debug和release的编译选项
set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

IF (CMAKE_BUILD_TYPE MATCHES "DEBUG")
    message(STATUS "CMAKE_CXX_FLAGS_DEBUG:" ${CMAKE_CXX_FLAGS_DEBUG})
ELSE(CMAKE_BUILD_TYPE MATCHES "DEBUG")
    message(STATUS "CMAKE_CXX_FLAGS_RELEASE:" ${CMAKE_CXX_FLAGS_RELEASE})
ENDIF (CMAKE_BUILD_TYPE MATCHES "DEBUG")

# 头文件目录
include_directories("../Base")
include_directories("../Common")

# lib库目录
#link_directories("/home/acc/app/boost_1_64_0/stage/lib")

# 查找目录下的所有源文件
# 并将名称保存到 SRCLIST 变量
aux_source_directory(../Base SRCLIST)

# 指定生成目标
add_library(Base SHARED ${SRCLIST})

# 添加链接库
#target_link_libraries(Base pthread)
