cmake_minimum_required(VERSION 3.26)

project(Yadro_tape_task)

set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS        OFF)

set(CMAKE_CXX_FLAGS_DEBUG "-g -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -Wno-deprecated-declarations")

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

find_package(GTest REQUIRED)
enable_testing()

add_executable(Yadro_tape_task ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
                               ${CMAKE_CURRENT_SOURCE_DIR}/source/config.cpp)
add_executable(Yadro_tape_task_test ${CMAKE_CURRENT_SOURCE_DIR}/tests/test.cpp
                                    ${CMAKE_CURRENT_SOURCE_DIR}/source/config.cpp)

target_link_libraries(Yadro_tape_task_test PRIVATE ${GTEST_LIBRARIES})

target_include_directories(Yadro_tape_task PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(Yadro_tape_task_test PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)