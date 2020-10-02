# 设置CMAKE最小版本
cmake_minimum_required(VERSION 3.5)

# 可执行文件名的名称
set(EXECUTABLE_EXE_NAME MyAlgo)

# 可执行文件生成路径
set(EXECUTABLE_OUTPUT_PATH ../bin)
# 测试可执行文件运行路径
# set(UNIT_TEST_BIN_OUTPUT_DIR ${EXECUTABLE_OUTPUT_PATH})

# 设置工程名称
project (${EXECUTABLE_EXE_NAME})

# main函数
set(SOURCES
    src/main.cc
)

# 单元测试的文件
set(UNITTEST_SOURCE
)

# 把源文件添加进工程中
add_executable(${EXECUTABLE_EXE_NAME} ${SOURCES}
                                      ${UNITTEST_SOURCE}
)

# 设置包含路径
target_include_directories(${EXECUTABLE_EXE_NAME}
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)

if(MSVC)
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4") # 设置警告等级为4
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /WX") # 设置警告视为错误
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /std:c++latest") # 设置使用最新C++标准
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
# 设置VS运行库为/MD和/MTD
set(CompilerFlags
    CMAKE_CXX_FLAGS
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
    CMAKE_C_FLAGS
    CMAKE_C_FLAGS_DEBUG
    CMAKE_C_FLAGS_RELEASE
    )
foreach(CompilerFlag ${CompilerFlags})
    string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()
endif()

# 设置GCC最高警告级别
if (CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++2a")
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -lprofiler")
endif()

#设置C++标准
# set(CMAKE_CXX_STANDARD 20)
