
cmake_minimum_required(VERSION 3.0)
project(NFServer)

file(GLOB NFServer_ROOT_Cpp *.cpp)
file(GLOB NFServer_ROOT_Hpp *.h)

add_executable(NFServer ${NFServer_ROOT_Cpp} ${NFServer_ROOT_Hpp})

set_target_properties(NFServer PROPERTIES LINKER_LANGUAGE CXX)

if( CMAKE_BUILD_TYPE STREQUAL "Release" )
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -std=c99 -fPIC -g -O2 -Wall")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++2a -g -O2 -Wall -fcoroutines-ts")
else()
    set(CMAKE_BUILD_TYPE "Debug")
    set(CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -std=c99 -O0 -Wall -g -ggdb -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a -O0 -Wall -g -ggdb -fPIC -fcoroutines-ts")
endif()
