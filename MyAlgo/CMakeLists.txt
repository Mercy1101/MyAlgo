project(MyAlgo)

# Create the executable
add_executable(${PROJECT_NAME} 
            main.cpp
            src/gos/gos_func.cpp
            unittest/gos_func_unittest.cpp
            unittest/algorithm_unittest.cpp
)

target_include_directories(MyAlgo
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/include/gos
)

target_link_libraries(${PROJECT_NAME}
    sub::googletest
)
