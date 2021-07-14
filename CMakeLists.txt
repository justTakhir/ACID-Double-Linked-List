cmake_minimum_required(VERSION 3.2)

project(consistent-list)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(consistent-list INTERFACE)
target_include_directories(consistent-list INTERFACE ${PROJECT_SOURCE_DIR}/src)
target_sources(consistent-list INTERFACE
    ${PROJECT_SOURCE_DIR}/src/ConsistentList.hpp
    ${PROJECT_SOURCE_DIR}/src/ConsistentListException.hpp
    ${PROJECT_SOURCE_DIR}/src/ConstIterator.hpp
    ${PROJECT_SOURCE_DIR}/src/Iterator.hpp
    ${PROJECT_SOURCE_DIR}/src/Node.hpp
    ${PROJECT_SOURCE_DIR}/src/SentinelNode.hpp
    ${PROJECT_SOURCE_DIR}/src/TrueNode.hpp
    ${PROJECT_SOURCE_DIR}/src/SpinlockBasedRWLock.hpp
    ${PROJECT_SOURCE_DIR}/src/GarbageCollector.hpp
) 

enable_testing()

add_executable(consistent-list-tests Tests.cpp IteratorTests.cpp ConstIteratorTests.cpp MultiThreadTests.cpp)
target_link_libraries(consistent-list-tests consistent-list gtest_main)

include(GoogleTest)
gtest_discover_tests(consistent-list-tests)