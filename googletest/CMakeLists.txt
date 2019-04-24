# Set the project name
project (googletest)

# file(GLOB_RECURSE HDRS ${PROJECT_SOURCE_DIR}/include)
# Add a library with the above sources
add_library(${PROJECT_NAME} 
    src/gtest.cc
    src/gtest_main.cc
    src/gtest-all.cc
    src/gtest-death-test.cc
    src/gtest-filepath.cc
    src/gtest-matchers.cc
    src/gtest-port.cc
    src/gtest-printers.cc
    src/gtest-test-part.cc
    src/gtest-typed-test.cc)
add_library(sub::googletest ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PROJECT_SOURCE_DIR}
)
