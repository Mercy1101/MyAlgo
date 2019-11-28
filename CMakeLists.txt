# 设置CMAKE最小版本
cmake_minimum_required(VERSION 3.5)

# 可执行文件名的名称
set(EXECUTABLE_EXE_NAME MyAlgo)

# 可执行文件生成路径
set(EXECUTABLE_OUTPUT_PATH ../bin)

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

# 单元测试模块源文件
set(GTEST_SOURCES 
    src/gtest/gtest.cc
    #src/googletest/gtest_main.cc
    #src/googletest/gtest-all.cc
    src/gtest/gtest-death-test.cc
    src/gtest/gtest-filepath.cc
    #src/gtest/gtest-matchers.cc
    src/gtest/gtest-port.cc
    src/gtest/gtest-printers.cc
    src/gtest/gtest-test-part.cc
    src/gtest/gtest-typed-test.cc
)

# spdlog的一个源文件
set(SPD_LOG_SOURCES
    include/tools/log/spdlog.cpp
)

# 单元测试的文件
set(UNITTEST_SOURCE
  # 自己实现的Singleton的单元测试文件
  unittest/tools/designpattern/Singleton_unittest.cpp

  # 自己封装的log模块的单元测试文件
  unittest/tools/log/log_unittest.cpp

  # 自己写的工具集utility的单元测试文件
  unittest/tools/utility_unittest.cpp

  # 开源库concurrentqueue的例子
  unittest/concurrentqueue/concurrentqueue_unittest.cpp
)


# 把源文件添加进工程中
add_executable(${EXECUTABLE_EXE_NAME} ${SOURCES}
                                      ${GTEST_SOURCES}
                                      ${INIT_FILE_SOURCES}
                                      ${UNITTEST_SOURCE}
                                      ${SPD_LOG_SOURCES}
                                  
)

# 设置包含路径
target_include_directories(${EXECUTABLE_EXE_NAME}
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include

        # 单元测试模块的包含路径
        ${PROJECT_SOURCE_DIR}/include/gtest
        ${PROJECT_SOURCE_DIR}/src/gtest
        
        # 无锁消息池队列实现
        ${PROJECT_SOURCE_DIR}/include/concurrentqueue
        
        # 自己封装的小工具的路径
        ${PROJECT_SOURCE_DIR}/include/tools

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
target_compile_definitions(${EXECUTABLE_EXE_NAME} PRIVATE SPDLOG_COMPILED_LIB)
# 为了屏蔽gtest中使用了TR1产生的警告,定义该宏
target_compile_definitions(${EXECUTABLE_EXE_NAME} PRIVATE _SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING)

# debug模式下,定义_DEBUG宏
#IF (CMAKE_BUILD_TYPE STREQUAL Debug)
#ADD_DEFINITIONS(CONFIG=Debug -D_DEBUG)
#ENDIF()


# ADD_DEFINITIONS(-_CRT_SECUER_NO_WARNINGS)

