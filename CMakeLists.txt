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

# 添加静态库的连接路径
#link_directories(
                 # Boost库的路径
                 # $ENV{BOOST_LIB_DIR})

# main函数
set(SOURCES
    src/main.cpp
)

# 初始化文件
set(INIT_FILE_SOURCES src/init/init.cpp)

# spdlog的一个源文件
set(SPD_LOG_SOURCES
    include/tools/log/spdlog.cpp
    include/tools/log/stdout_sinks.cpp
    include/tools/log/spdlog.cpp
    include/tools/log/fmt.cpp
    include/tools/log/file_sinks.cpp
    include/tools/log/color_sinks.cpp
    include/tools/log/async.cpp
)

# leetcode源文件
set(LEET_CODE_FILE_SOURCES src/leetcode/leetcode.cpp)

# tools文件夹下的源文件
set(TOOLS_FILE_SOURCES src/tools/data_struct/print_data_struct.cpp)

# 单元测试的文件
set(UNITTEST_SOURCE
  # 自己实现的 Singleton 的单元测试文件
  unittest/tools/designpattern/Singleton_unittest.cpp

  # 自己封装的 log 模块的单元测试文件
  unittest/tools/log/log_unittest.cpp

  # 自己写的工具集utility 的单元测试文件
  unittest/tools/utility/utility_unittest.cpp

  # 开源库 concurrentqueue 的例子
  unittest/concurrentqueue/concurrentqueue_unittest.cpp

  # leetcode 解法测试
  unittest/leetcode/leetcode_unittest.cpp

  # utility/time 的测试单元文件
  unittest/tools/utility/time_utility_unittest.cpp

  # utility/print_data_struct 的测试单元文件
  unittest/tools/data_struct/print_data_struct_unittest.cpp

  # utility/binary_tree 的测试单元文件
  unittest/tools/data_struct/binary_tree_unittest.cpp

  # utility/file_line_reader 的测试单元文件
  # unittest/tools/utility/file_line_reader_utility_unittest.cpp

  # profiler 模块的测试文件
  unittest/tools/profiler/profiler_unittest.cpp

  # 并行工具的单元测试文件
  unittest/tools/utility/concurrency_utility_unittest.cpp

  # 四则运算的测试单元文件
  unittest/tools/data_struct/calculate_unittest.cpp
)

# 把源文件添加进工程中
add_executable(${EXECUTABLE_EXE_NAME} ${SOURCES}
                                      ${INIT_FILE_SOURCES}
                                      ${UNITTEST_SOURCE}
                                      ${LEET_CODE_FILE_SOURCES}
                                      ${TOOLS_FILE_SOURCES}
)

# 设置包含路径
target_include_directories(${EXECUTABLE_EXE_NAME}
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include

        # 无锁消息池队列实现
        ${PROJECT_SOURCE_DIR}/include/concurrentqueue
        
        # 自己封装的小工具的路径
        ${PROJECT_SOURCE_DIR}/include/tools

        # leetcode 文件的头文件路径
        ${PROJECT_SOURCE_DIR}/include/leetcode

        # 添加spdlog模块的包含路径
        ${PROJECT_SOURCE_DIR}/include/tools/log

        # boost库的包含路径
        #$ENV{BOOST_ROOT}
)

# 东进驱动函数的lib文件
#set(DONGJIN_API_LIB 
#  NewSig.lib
#  Tc08a32.lib)

#target_link_libraries(${EXECUTABLE_EXE_NAME} ${DONGJIN_API_LIB})

# 设置VS警告等级为Warning4
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

# 设置VS运行库为/MD和/MTD
if (MSVC)
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
endif(MSVC)

# 定义这个宏,是为了正常编译spdlog
#target_compile_definitions(${EXECUTABLE_EXE_NAME} PRIVATE SPDLOG_COMPILED_LIB)
#target_compile_definitions(${EXECUTABLE_EXE_NAME} PRIVATE SPDLOG_HEADER_ONLY)
target_compile_definitions(${EXECUTABLE_EXE_NAME} PRIVATE CATCH_CONFIG_ENABLE_BENCHMARKING)


# 定义这个宏为了避免dbg文件中的警告
ADD_DEFINITIONS(-DDBG_MACRO_NO_WARNING)
# ADD_DEFINITIONS(-_CRT_SECUER_NO_WARNINGS)

# 设置GCC最高警告级别
if ( CMAKE_COMPILER_IS_GNUCC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall")
endif()
# 设置MSVC最高警告级别
if ( MSVC )
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /W4") # 设置警告等级为4
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /WX") # 设置警告视为错误
    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} /std:c++latest") # 设置使用最新C++标准
endif()


#enable_testing()
# add_test(NAME MyAlgo COMMAND MyAlgo WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR})
#add_test(NAME MyAlgo COMMAND -C Release WORKING_DIRECTORY ${UNIT_TEST_BIN_OUTPUT_DIR})


