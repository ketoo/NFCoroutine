cmake_minimum_required(VERSION 3.8)

set(ProjectName "NFCoroutinePlugin")
file(GLOB NFCoroutinePlugin_ROOT_Cpp
        *.cpp)

file(GLOB NFCoroutinePlugin_ROOT_Hpp
        *.h)


set(CMAKE_CXX_STANDARD 11)

add_executable(ProjectName ${NFCoroutinePlugin_ROOT_Cpp}
        ${NFCoroutinePlugin_ROOT_Hpp})

