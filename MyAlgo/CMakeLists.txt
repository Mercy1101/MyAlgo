project(MyAlgo)

# Create the executable
add_executable(${PROJECT_NAME} 
            main.cpp
            src/list.cpp
            src/list_unittest.cpp
            src/easylogging++.cpp
            src/profiler.cpp
)

target_include_directories(MyAlgo
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/googletest/include
)

# Link the static library from subproject1 using it's alias sub::lib1
# Link the header only library from subproject2 using it's alias sub::lib2
# This will cause the include directories for that target to be added to this project
target_link_libraries(${PROJECT_NAME}
    sub::googletest
)
