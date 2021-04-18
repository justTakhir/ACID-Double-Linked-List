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

add_library(consistent-list
    ConsistentList.hpp
    ConsistentListException.hpp
    ConstIterator.hpp
    Iterator.hpp
    Node.hpp
    SentinelNode.hpp
    TrueNode.hpp
)

set_target_properties(consistent-list PROPERTIES LINKER_LANGUAGE CXX)

enable_testing()

add_executable(consistent-list-tests Tests.cpp)
target_link_libraries(consistent-list-tests gtest_main)

include(GoogleTest)
gtest_discover_tests(consistent-list-tests)