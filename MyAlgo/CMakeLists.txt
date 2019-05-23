project(MyAlgo)

# Create the executable
add_executable(${PROJECT_NAME} 
            main.cpp
            src/list.cpp
            src/gos_itoa.cpp
            src/gos_get_localtime.cpp
            src/gos_get_file_size.cpp
            unittest/list_unittest.cpp
            unittest/gos_itoa_unittest.cpp
            unittest/gos_get_localtime_unittest.cpp
            unittest/gos_get_file_size_unittest.cpp
            src/profiler.cpp
            src/easylogging++.cpp
)

target_include_directories(MyAlgo
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/googletest/include
)

target_link_libraries(${PROJECT_NAME}
    sub::googletest
)
